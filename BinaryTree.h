#pragma once

#include <iostream>
#include <utility>
#include <functional>
#include <stack>
#include <queue>

template <typename _Ty>
class BinaryTree
{
private:
	struct Node
	{
		_Ty _data;
		Node* _left;
		Node* _right;
		Node* _parent;

		Node(const _Ty& value)
			: _data(value)
			, _left(nullptr)
			, _right(nullptr)
			, _parent(nullptr)
		{
		}
	};

	Node* root;


	Node* copy(Node* other_root, Node* parent);
	void destroy(Node* node);
	std::size_t height(Node* node) const;
	std::size_t size(Node* node) const;

public:
	using value_type = _Ty;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	class Iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = _Ty;
		using difference_type = std::ptrdiff_t;
		using pointer = _Ty*;
		using reference = _Ty&;

		Iterator();
		~Iterator();
		Iterator(Node* root);

		reference operator*() const;
		pointer operator->() const;

		Iterator& operator++();
		Iterator operator++(int);

		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;

	private:
		Node* _node = nullptr;
		std::stack<Node*> _stack;

		void advence();

		void pushLeft(Node* node);
	};

	class ConstIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = _Ty;
		using difference_type = std::ptrdiff_t;
		using pointer = const _Ty*;
		using reference = const _Ty&;

		ConstIterator();
		~ConstIterator();
		ConstIterator(const Iterator& it);

		reference operator*() const;
		pointer operator->() const;

		ConstIterator& operator++();
		ConstIterator operator++(int);

		bool operator==(const ConstIterator& other) const;
		bool operator!=(const ConstIterator& other) const;

	private:
		Iterator _it;
	};

	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	BinaryTree();
	~BinaryTree();
	BinaryTree(const BinaryTree& other);
	BinaryTree(BinaryTree&& other) noexcept;
	BinaryTree(std::initializer_list<value_type> init);

	BinaryTree& operator=(const BinaryTree& other);
	BinaryTree& operator=(BinaryTree&& other) noexcept;

	void insert(const_reference value);
	bool contains(const_reference value) const;
	void remove(const_reference value);

	size_type size() const;
	size_type height() const;
	bool empty() const;

	void inorder(std::function<void(const_reference)> visit) const;
	void preorder(std::function<void(const_reference)> visit) const;
	void postorder(std::function<void(const_reference)> visit) const;
	void levelorder(std::function<void(const_reference)> visit) const;

	void clear();
	void printTree() const;

	void swap(BinaryTree& other) noexcept;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
};

template<typename _Ty>
typename BinaryTree<_Ty>::Node* BinaryTree<_Ty>::copy(Node* other_root, Node* parent)
{
	if (!other_root)
		return;

	Node* node = new Node(other_root->_data);
	node->_parent = parent;
	node->_left = copy(other_root->_left, node);
	node->_right = copy(other_root->_right, node);
	return node;
}

template<typename _Ty>
inline void BinaryTree<_Ty>::destroy(Node* node)
{
	if (!node)
		return;

	destroy(node->_left);
	destroy(node->_right);
	delete node;
}

template<typename _Ty>
inline std::size_t BinaryTree<_Ty>::height(Node* node) const
{
	if (!node)
		return 0;
	return 1 + std::max(height(node->_left), height(node->_right));
}

template<typename _Ty>
inline std::size_t BinaryTree<_Ty>::size(Node* node) const
{
	if (!node)
		return 0;
	return 1 + size(node->_left) + size(node->_right);
}

template<typename _Ty>
inline BinaryTree<_Ty>::BinaryTree()
	: root(nullptr)
{
}

template<typename _Ty>
inline BinaryTree<_Ty>::~BinaryTree()
{
	clear();
}

template<typename _Ty>
inline BinaryTree<_Ty>::BinaryTree(const BinaryTree& other)
	: root(nullptr)
{
	root = copy(other.root, nullptr);
}

template<typename _Ty>
inline BinaryTree<_Ty>::BinaryTree(BinaryTree&& other) noexcept
	: root(other.root)
{
	other.root = nullptr;
}

template<typename _Ty>
inline BinaryTree<_Ty>::BinaryTree(std::initializer_list<value_type> init)
	: root(nullptr)
{
	for (const auto& elem : init)
		insert(elem);
}

template<typename _Ty>
inline BinaryTree<_Ty>& BinaryTree<_Ty>::operator=(const BinaryTree& other)
{
	if (this != &other)
	{
		BinaryTree temp(other);
		swap(temp);
	}
	return *this;
}

template<typename _Ty>
inline BinaryTree<_Ty>& BinaryTree<_Ty>::operator=(BinaryTree&& other) noexcept
{
	if (this != &other)
	{
		clear();
		root = other.root;
		other.root = nullptr;
	}
	return *this;
}

template<typename _Ty>
inline void BinaryTree<_Ty>::insert(const_reference value)
{
	Node* new_node = new Node(value);
	if (!root)
	{
		root = new_node;
		return;
	}

	Node* current = root;
	Node* parent = nullptr;
	while (current)
	{
		parent = current;
		if (value < current->_data)
			current = current->_left;
		else
			current = current->_right;
	}
	new_node->_parent = parent;
	if (value < parent->_data)
		parent->_left = new_node;
	else
		parent->_right = new_node;
}

template<typename _Ty>
inline bool BinaryTree<_Ty>::contains(const_reference value) const
{
	Node* current = root;
	while (current)
	{
		if (value == current->_data)
			return true;
		current = value < current->_data ? current->_left : current->_right;
	}
	return false;
}

template<typename _Ty>
inline void BinaryTree<_Ty>::remove(const_reference value)
{
	Node* parent = nullptr;
	Node* current = root;

	while (current && current->_data != value)
	{
		parent = current;
		current = value < current->_data ? current->_left : current->_right;
	}

	if (!current)
		return;

	if (!current->_left || !current->_right)
	{
		Node* child = current->_left ? current->_left : current->_right;
		if (!parent)
			root = child;
		else if (parent->_left == current)
			parent->_left = child;
		else
			parent->_right = child;
		if (child)
			child->_parent = parent;
		delete current;
	}
	else
	{
		Node* succParent = current;
		Node* succ = current->_right;

		while (succ->_left)
		{
			succParent = succ;
			succ = succ->_left;
		}
		current->_data = succ->_data;
		if (succParent->_left == succ)
			succParent->_left = succ->_right;
		else
			succParent->_right = succ->_right;
		if (succ->_right) 
			succ->_right->_parent = succParent;
		delete succ;
	}
}

template <typename _Ty>
typename BinaryTree<_Ty>::size_type BinaryTree<_Ty>::size() const
{
	return size(root);
}

template<typename _Ty>
typename BinaryTree<_Ty>::size_type BinaryTree<_Ty>::height() const
{
	return height(root);
}

template<typename _Ty>
inline void BinaryTree<_Ty>::inorder(std::function<void(const_reference)> visit) const
{
	std::function<void(Node*)> dfs = [&](Node* node)
		{
			if (!node)
				return;
			dfs(node->_left);
			visit(node->_data);
			dfs(node->_right);
		};
	dfs(root);
}

template<typename _Ty>
inline void BinaryTree<_Ty>::preorder(std::function<void(const_reference)> visit) const
{
	std::function<void(Node*)> dfs = [&](Node* node)
		{
			if (!node)
				return;
			visit(node->_data);
			dfs(node->_left);
			dfs(node->_right);
		};
	dfs(root);
}

template<typename _Ty>
inline void BinaryTree<_Ty>::postorder(std::function<void(const_reference)> visit) const
{
	std::function<void(Node*)> dfs = [&](Node* node)
		{
			if (!node)
				return;
			dfs(node->_left);
			dfs(node->_right);
			visit(node->_data);
		};
	dfs(root);
}

template<typename _Ty>
inline void BinaryTree<_Ty>::levelorder(std::function<void(const_reference)> visit) const
{
	if (!root)
		return;
	std::queue<Node*> q;
	q.push(root);
	while (!q.empty())
	{
		Node* node = q.front();
		q.pop();
		visit(node->_data);
		if (node->_left)
			q.push(node->_left);
		if (node->_right)
			q.push(node->_right);
	}
}

template<typename _Ty> 
void BinaryTree<_Ty>::clear()
{
	destroy(root);
	root = nullptr;
}

template<typename _Ty>
inline void BinaryTree<_Ty>::printTree() const
{
	inorder([](const_reference value)
		{
			std::cout << value << " ";
		});
	std::cout << "\n";
}

template<typename _Ty>
inline void BinaryTree<_Ty>::swap(BinaryTree& other) noexcept
{
	std::swap(root, other.root);
}
 
template<typename _Ty>
inline void BinaryTree<_Ty>::Iterator::advence()
{
	if (!_stack.empty())
	{
		_node = _stack.top();
		_stack.pop();
		pushLeft(_node->_right);
	}
	else
		_node = nullptr;
}

template<typename _Ty>
inline void BinaryTree<_Ty>::Iterator::pushLeft(Node* node)
{
	while (node)
	{
		_stack.push(node);
		node = node->_left;
	}
}

template<typename _Ty>
inline BinaryTree<_Ty>::Iterator::Iterator()
	: _node(nullptr)
{
}

template<typename _Ty>
inline BinaryTree<_Ty>::Iterator::~Iterator()
{
}

template<typename _Ty>
inline BinaryTree<_Ty>::Iterator::Iterator(Node* root)
{
	pushLeft(root);
	advence();
}

template <typename _Ty>
typename BinaryTree<_Ty>::Iterator::reference BinaryTree<_Ty>::Iterator::operator*() const
{
	return _node->_data;
}

template <typename _Ty>
typename BinaryTree<_Ty>::Iterator::pointer BinaryTree<_Ty>::Iterator::operator->() const
{
	return &_node->_data;
}

template <typename _Ty>
typename BinaryTree<_Ty>::Iterator& BinaryTree<_Ty>::Iterator::operator++()
{
	advence();
	return *this;
}

template <typename _Ty>
typename BinaryTree<_Ty>::Iterator BinaryTree<_Ty>::Iterator::operator++(int)
{
	Iterator temp = *this;
	++(*this);
	return temp;
}

template<typename _Ty>
inline bool BinaryTree<_Ty>::Iterator::operator==(const Iterator& other) const
{
	return _node == other._node;
}

template<typename _Ty>
inline bool BinaryTree<_Ty>::Iterator::operator!=(const Iterator& other) const
{
	return _node != other._node;
}

template<typename _Ty>
inline BinaryTree<_Ty>::ConstIterator::ConstIterator()
	: _it()
{
}

template<typename _Ty>
inline BinaryTree<_Ty>::ConstIterator::~ConstIterator()
{
}

template<typename _Ty>
inline BinaryTree<_Ty>::ConstIterator::ConstIterator(const Iterator& it)
	: _it(it)
{
}

template <typename _Ty>
typename BinaryTree<_Ty>::ConstIterator::reference BinaryTree<_Ty>::ConstIterator::operator*() const
{
	return *_it;
}

template <typename _Ty>
typename BinaryTree<_Ty>::ConstIterator::pointer BinaryTree<_Ty>::ConstIterator::operator->() const
{
	return _it.operator->();
}

template <typename _Ty>
typename BinaryTree<_Ty>::ConstIterator& BinaryTree<_Ty>::ConstIterator::operator++()
{
	++_it;
	return *this;
}

template <typename _Ty>
typename BinaryTree<_Ty>::ConstIterator BinaryTree<_Ty>::ConstIterator::operator++(int)
{
	ConstIterator temp = *this;
	++(*this);
	return temp;
}

template<typename _Ty>
inline bool BinaryTree<_Ty>::ConstIterator::operator==(const ConstIterator& other) const
{
	return _it == other._it;
}

template<typename _Ty>
inline bool BinaryTree<_Ty>::ConstIterator::operator!=(const ConstIterator& other) const
{
	return _it != other._it;
}

template <typename _Ty>
typename BinaryTree<_Ty>::iterator BinaryTree<_Ty>::begin()
{
	return iterator(root);
}

template <typename _Ty>
typename BinaryTree<_Ty>::iterator BinaryTree<_Ty>::end()
{
	return iterator();
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_iterator BinaryTree<_Ty>::begin() const
{
	return const_iterator(root);
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_iterator BinaryTree<_Ty>::end() const
{
	return const_iterator();
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_iterator BinaryTree<_Ty>::cbegin() const
{
	return const_iterator(root);
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_iterator BinaryTree<_Ty>::cend() const
{
	return const_iterator();
}

template <typename _Ty>
typename BinaryTree<_Ty>::reverse_iterator BinaryTree<_Ty>::rbegin()
{ 
	return reverse_iterator(end()); 
}

template <typename _Ty>
typename BinaryTree<_Ty>::reverse_iterator BinaryTree<_Ty>::rend()
{ 
	return reverse_iterator(begin()); 
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_reverse_iterator BinaryTree<_Ty>::rbegin() const
{ 
	return const_reverse_iterator(end());
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_reverse_iterator BinaryTree<_Ty>::rend() const
{ 
	return const_reverse_iterator(begin()); 
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_reverse_iterator BinaryTree<_Ty>::crbegin() const
{ 
	return const_reverse_iterator(cend()); 
}

template <typename _Ty>
typename BinaryTree<_Ty>::const_reverse_iterator BinaryTree<_Ty>::crend() const
{ 
	return const_reverse_iterator(cbegin()); 
}