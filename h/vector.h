#ifndef _vector_h_
#define _vector_h_

template<class T>
class Vector {
private:
	struct Node {
		T info;
		Node *prev;
		Node *next;

		Node(T info, Node *prev = nullptr, Node *next = nullptr) : info(info), prev(prev), next(next) {}
	};

	Node *head;
	Node *tail;
	Node *walk;

public:
	unsigned size;

	Vector() : head(nullptr), tail(nullptr), walk(nullptr), size(0) {}

	~Vector() {
		Node *next;

		lock;
		while (head != nullptr) {
			next = head->next;

			delete head;

			head = next;
		}
		unlock;
	}

	void pushf(T info) {
		lock;
		if (tail == nullptr) head = tail = new Node(info);
		else {
			tail->next = new Node(info, tail);
			tail = tail->next;
		}

		size++;
		unlock;
	}

	void pushb(T info) {
		lock;
		if (head == nullptr) head = tail = new Node(info);
		else {
			head->prev = new Node(info, nullptr, head);
			head = head->prev;
		}

		size++;
		unlock;
	}

	T popf() {
		lock;
		T info = tail->info;

		if (walk == tail) walk = tail->prev;

		if (tail->prev != nullptr) {
			tail = tail->prev;
			delete tail->next;
			tail->next = nullptr;
		} else {
			delete tail;
			head = tail = nullptr;
		}

		size--;
		unlock;

		return info;
	}

	T popb() {
		lock;
		T info = head->info;

		if (walk == head) walk = head->next;

		if (head->next != nullptr) {
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
		} else {
			delete head;
			head = tail = nullptr;
		}

		size--;
		unlock;

		return info;
	}

	void operator++(int) {
		if (walk) walk = walk->next;
	}

	void operator--(int) {
		if (walk) walk = walk->prev;
	}

	void operator++() {
		if (tail) walk = tail;
	}

	void operator--() {
		if (head) walk = head;
	}

	T get() {
		if (walk) {
			return walk->info;
		} else {
			// cout << "Err" << endl;
			return 0;
		}
	}

	void remove() {
		if (walk == nullptr) return;

		lock;
		if (walk == head) head = head->next;
		if (walk == tail) tail = tail->prev;

		if (walk->prev) walk->prev->next = walk->next;
		if (walk->next) walk->next->prev = walk->prev;

		delete walk;

		walk = head;
		size--;
		unlock;

	}
};

#endif
