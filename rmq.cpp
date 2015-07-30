
// g++ -std=c++11 rmq.cpp -o rmq -g

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <sstream>

#include <cmath>



class CrmqCalculater {
	typedef std::function<const size_t& (const size_t&, const size_t&)> CmpFunc;
public:
	CrmqCalculater () {}
	template <typename Iter>
	CrmqCalculater (
		Iter start,
		Iter end,
		CmpFunc f
	)
	{
		Initialize(start, end, f);
		return;
	}
	
	virtual ~CrmqCalculater() {}
	
	template <typename Iter>
	void ReInitialize(
		Iter start,
		Iter end,
		CmpFunc f
	)
	{
		Initialize(start, end, f);
		return;
	}
	
	template <typename Iter>
	void Initialize(
		Iter start,
		Iter end,
		CmpFunc f
	)
	{
		if (!m_data.empty()) {
			m_data.clear();
			m_sparse_table.clear();
			m_2log.clear();
		}
		
		while (start != end) {
			m_data.push_back(*start);
			++start;
		}
		m_cmp = f;
		
		CalcSparseTable();
		Calc2LogTable();
		
		return;
	}
	
	size_t GetQueryResult(size_t i, size_t j) const {
		if (i > j || i >= m_data.size() || j >= m_data.size())
			throw std::runtime_error("Incorrect index values");
		if (i == j)
			return m_data[i];
		
		size_t log_val = m_2log[(j - i + 1) - 1];
/*
for (size_t i = 0; i < m_sparse_table.size(); ++i) {
	for (size_t j = 0; j < m_sparse_table[i].size(); ++j)
		std::cout << m_sparse_table[i][j] << ' ';
	std::cout << "\n";
}
std::cout << "\n\n" << (j - i + 1) - 1 << " - " << log_val << " - " << m_sparse_table.size() << "\n";
*/
		if (log_val >= m_sparse_table.size())
			throw std::runtime_error("Incorrect calculations");
		
		//size_t val = std::min(
		size_t val = m_cmp(
			m_sparse_table[log_val][i],
			m_sparse_table[log_val][j - pow(2, log_val) + 1]
		);
		
		return val;
	}
	
protected:
	virtual size_t CalcFloor2Log (size_t val) {
		size_t cnt = 0;
		
		while(val) {
			if (val == 1)
				break;
			
			val /= 2;
			++cnt;
		}
		
		return cnt;
	}

private:
	virtual void CalcSparseTable();
	virtual void Calc2LogTable();
	
private:
	std::vector <size_t> m_data;
	std::vector <std::vector<size_t> > m_sparse_table;
	std::vector <size_t> m_2log;
	
	CmpFunc m_cmp;
};


void CrmqCalculater::Calc2LogTable() {
	size_t i, j;
	
	i = 1;
	j = m_data.size();
	
	while (i <= j) {
		m_2log.push_back(CalcFloor2Log(i));
		++i;
	}
	
	return;
}


void CrmqCalculater::CalcSparseTable() {
	m_sparse_table.push_back(m_data);
	size_t j = CalcFloor2Log(m_data.size());
	size_t i = 1, data_size = m_data.size();
	
	while(i <= j) {
		size_t pow_val = std::pow(2, i);
		size_t pow_val_min1 = std::pow(2, i - 1);
		std::vector<size_t> new_row;
		new_row.reserve(m_data.size() - pow_val + 1);
		
		for (size_t k = 0; k + pow_val <= data_size; ++k) {
			new_row.push_back(
				//std::min(
				m_cmp(
					m_sparse_table[i - 1][k],
					m_sparse_table[i - 1][k + pow_val_min1]
				)
			);
		}
		
		m_sparse_table.push_back(new_row);
		++i;
	}
	
	return;
}


int main (int argc, char *argv[]) {
	size_t i = 0, j = 0;
	std::vector<int> data {1,2,3,8,0,6,5,3,2,1,34,4}; // n = 12
	//std::vector<int> data {3, 1, 9, 7, 8, 5, 0, 8, 6};
	CrmqCalculater min_query(
		data.begin(),
		data.end(),
		[](const size_t& a, const size_t& b)->const size_t&{return a <= b ? a : b;}
	);
	
	if (argc < 2) {
		std::cout << "Enter i, j\n";
		return 1002;
	}
	
	std::istringstream ss_cnv;
	ss_cnv.str(argv[1]);
	ss_cnv >> i;
	ss_cnv.clear();
	ss_cnv.str(argv[2]);
	ss_cnv >> j;
	
	try {
		std::cout << "Minimum of range [" << i << " - " << j
				  << "] = " << min_query.GetQueryResult(i, j)
				  << std::endl;
	} catch (std::exception & Exc) {
		std::cout << "Have caught an exception: " << Exc.what() << "\n";
		return 1001;
	}
	
	return 0;
}










