#include "vector.h"

template <typename val_, typename cmp>
class heap {

	vector<val_> data;
	cmp CMP;

	void push_up(size_t ind) {
		while (ind && CMP(data[ind], data[(ind - 1) / 2])) {
			std::swap(data[ind], data[(ind - 1) / 2]);
			ind = (ind - 1) / 2;
		}
	}
	void push_down(size_t ind) {
		while (2 * ind + 1 < data.size()) {
			size_t mc = 2 * ind + 1;
			if (mc + 1 < data.size() && !CMP(data[mc], data[mc + 1]) )
				mc++;
			if (CMP(data[ind], data[mc]))break;
			std::swap(data[ind], data[mc]);
			ind = mc;
		}
	}

public:
    heap() {};

	void push(const val_& right) {
		data.push_back(right);
		push_up(data.size() - 1);
	}

    val_ top() {
        if (!data.size()) throw std::range_error("heap is empty");
        return data[0];
    }

	val_ extract() {
		if (!data.size()) 
			throw std::logic_error("Cant extract from empty heap");
		val_ res = data[0];
		data[0] = data[data.size() - 1];
		data.pop_back();
		push_down(0);
		return res;
	}
    bool empty() { return !data.size(); }
};



//насколько я понял, самый верхний слой в бин дереве должен возрастать (по степеням, не по ключам),
//а остальные уровни должны убывать
//Пример кучи (показаны степени):
//0     1       2       3
//      0       1   0   2   1   0
//              0       1   0
//                      0
template<class val_, class cmp>
class BinomialHeap {
public:
    
    BinomialHeap() : head(nullptr) {}

    ~BinomialHeap() {
        clear(head);
    }

    void push(const val_& node_) {
        Node* new_ = new Node(node_);
        head = merge(head, new_);
    }
    
    val_ top() const {
        if (!head) {
            throw std::logic_error("can find in empty heap");
        }

        Node* Tmp_Node = head;
        Node* h0 = head->bro;

        while (h0) {
            if (CMP(h0->data, Tmp_Node->data)) {
                Tmp_Node = h0;
            }
            h0 = h0->bro;
        }

        return Tmp_Node->data;
    }

    bool empty() { return !head; }

    val_ extract() {
        if (!head) {
            throw std::runtime_error("cant extract from empty heap");
        }
        Node* cmp_Node = head;
        Node* prev_good = nullptr;
        Node* h0 = head->bro;
        Node* prev = head;

        while (h0) {
            if (CMP(h0->data, cmp_Node->data)) {
                cmp_Node = h0;
                prev_good = prev;
            }
            prev = h0;
            h0 = h0->bro;
        }
        if (!prev_good) {
            head = cmp_Node->bro;
        }
        else {
            prev_good->bro = cmp_Node->bro;
        }
        Node* son0;
        Node* son = cmp_Node->son;
        Node* nos = nullptr;

        while (son) {
            Node* next = son->bro;
            son->bro = nos;
            nos = son;
            son = next;
        }

        son0 = nos;

        head = merge(head, son0);

        val_ res = cmp_Node->data;
        delete cmp_Node;
        return res;
    }

private:
    struct Node {
        val_ data;
        size_t degree;
        Node* son;
        Node* bro;
        Node(const val_& k) : data(k), degree(0), son(nullptr), bro(nullptr) {}
    };

    Node* head;
    cmp CMP;

    Node* merge(Node* left, Node* right) {
        Node* res = nullptr;
        Node* h0 = nullptr;
        Node* h1 = left;
        Node* h2 = right;

        while (h1 != nullptr && h2 != nullptr) {
            if (h1->degree <= h2->degree) {
                if (!res) res = h0 = h1;
                else {
                    h0->bro = h1;
                    h0 = h1;
                }
                h1 = h1->bro;
            }
            else {
                if (!res) res = h0 = h2;
                else {
                    h0->bro = h2;
                    h0 = h2;
                }
                h2 = h2->bro;
            }
        }

        while (h1) {
            if (!res) res = h0 = h1;
            else {
                h0->bro = h1;
                h0 = h1;
            }
            h1 = h1->bro;
        }
        while (h2) {
            if (!res) res = h0 = h2;
            else {
                h0->bro = h2;
                h0 = h2;
            }
            h2 = h2->bro;
        }
        return eq_merge(res);
    }

    Node* eq_merge(Node* head) {
        if (!head) return nullptr;

        Node* prev = nullptr;
        Node* curr = head;
        Node* next = head->bro;

        while (next) {
            if (curr->degree != next->degree) {
                prev = curr;
                curr = next;
                next = next->bro;
            }
            else {
                if (CMP(curr->data, next->data)) {
                    curr->bro = next->bro;
                    next->bro = curr->son;
                    curr->son = next;
                    curr->degree++;
                }
                else {
                    if (prev) {
                        prev->bro = next;
                    }
                    else {
                        head = next;
                    }
                    curr->bro = next->son;
                    next->son = curr;
                    next->degree++;
                    curr = next;
                }
                next = curr->bro;
            }
        }
        return head;
    }

    void clear(Node* node) {
        if (node) {
            clear(node->son);
            clear(node->bro);
            delete node;
        }
    }

    Node* find(const val_& key) {
        Node* h0 = head;

        while (h0) {
            if (h0->data == key)
                return h0;

            Node* res = findSub(key, h0->son);
            if (res)
                return res;

            h0 = h0->bro;
        }
        return nullptr;
    }


    Node* findSub(const val_& key, Node* right) {
        if (!right) return nullptr;

        if (right->data == key)
            return right;

        Node* res = findSub(key, right->son);
        if (res) return res;

        return findLvl(key, right->bro);

    }

    Node* findLvl(const val_& key, Node* bro) {
        if (!bro) return nullptr;
        if (bro->data == key) return bro;

        Node* res = findSub(key, bro->son);
        if (res) return res;

        return findSub(key, bro->bro);
    }
};