
// g++ -std=c++11 lca.cpp -o lca

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <tuple>



template <typename Key, typename Val>
class CTree {
public:
	class CNode {
	public:
		CNode(): m_key(Key()), m_val(Val()) {}
		CNode(Key k, Val v): m_key(k), m_val(v) {}
		CNode(Key k, Val v, std::shared_ptr<CNode> par): m_key(k), m_val(v), m_parent(par) {}
		CNode(
			Key k,
			Val v,
			std::shared_ptr<CNode> left,
			std::shared_ptr<CNode> right,
			std::shared_ptr<CNode> par
		): m_key(k),
		   m_val(v),
		   m_left(left),
		   m_right(right),
		   m_parent(par)
		{}
		
		void SetParent (std::shared_ptr<CNode> p) { m_parent = p;}
		void SetLeft (std::shared_ptr<CNode> p) {m_left = p;}
		void SetRight (std::shared_ptr<CNode> p) {m_right = p;}
		std::shared_ptr<CNode> GetParent () const {
			return m_parent.lock();
		}
		std::shared_ptr<CNode> GetLeft () const {
			return m_left;
		}
		std::shared_ptr<CNode> GetRight () const {
			return m_right;
		}
		
		Key GetKey() const {
			return m_key;
		}
		Val GetVal() const {
			return m_val;
		}
		
	private:
		Key m_key;
		Val m_val;
		
		std::shared_ptr<CNode> m_right;
		std::shared_ptr<CNode> m_left;
		std::weak_ptr<CNode> m_parent;
	};
	
	
	typedef std::shared_ptr<CNode> Ptr;
	
	CTree() {m_root->m_key = Key();}
	
	bool Insert () {}
	
private:
	Ptr m_root;
	size_t m_node_count;
};


typedef CTree<int, std::string>::CNode BaseNode;
typedef std::shared_ptr<BaseNode> BaseNodePtr;


std::tuple<BaseNodePtr, BaseNodePtr, BaseNodePtr> MakeSimpleTree() {
	BaseNodePtr root (new BaseNode(0, "root"));
	BaseNodePtr l1_l (new BaseNode(1, "1111", root));
	BaseNodePtr l1_r (new BaseNode(2, "2222", root));
	root->SetLeft(l1_l);
	root->SetRight(l1_r);
	
	
	BaseNodePtr l2_r(new BaseNode(3, "3333", l1_l));
	l1_l->SetRight(l2_r);
	BaseNodePtr l2_l(new BaseNode(4, "4444", l1_r));
	l1_r->SetLeft(l2_l);
	
	return std::make_tuple(root, l2_r, l2_l);
}


BaseNodePtr FindLca(BaseNodePtr root, BaseNodePtr node1, BaseNodePtr node2) {
	BaseNodePtr  ptr;
	size_t cnt1 = 0, cnt2 = 0;
	
	
	ptr = node1;
	while (ptr != root) {
		ptr = ptr->GetParent();
		++cnt1;
	}
	ptr = node2;
	while (ptr != root) {
		ptr = ptr->GetParent();
		++cnt2;
	}
	
	if (cnt1 > cnt2) {
		while (cnt1 != cnt2) {
			node1 = node1->GetParent();
			--cnt1;
		}
	} else if (cnt1 < cnt2) {
		while (cnt1 != cnt2) {
			node2 = node2->GetParent();
			--cnt2;
		}
	}
	
	while(node1 != node2) {
		node1 = node1->GetParent();
		node2 = node2->GetParent();
	}
	
	
	return node1;
}


int main () {
	std::tuple<BaseNodePtr, BaseNodePtr, BaseNodePtr> dat;
	dat = MakeSimpleTree();
	BaseNodePtr lca = FindLca(std::get<0>(dat), std::get<1>(dat), std::get<2>(dat));
	
	std::cout << lca->GetKey() << "; " << lca->GetVal() << "\n";
	
	return 0;
}

