#include <iostream>
#include <vector>
#include <queue>

struct Node
{
	int info;
	int height;
	Node* parent;
	Node* left;
	Node* right;
	Node()
	{
		this->info = 0;
		this->height = 1;
		this->parent = nullptr;
		this->left = nullptr;
		this->right = nullptr;
	}
};

struct AVL
{
	Node* root;
	Node* sentinel;

	AVL()
	{
		this->sentinel = new Node;
		this->root = sentinel;
		this->sentinel->height = 0;
	}

	~AVL()
	{
		CLEAR();
		delete sentinel;
	}

	int BF(Node* x)
	{
		return x->right->height - x->left->height;
	}

	void INSERT(int key)
	{
		Node* x = root;
		Node* y = sentinel;
		while (x != sentinel)
		{
			y = x;
			if (key < x->info)
			{
				x = x->left;
			}
			else if (key > x->info)
			{
				x = x->right;
			}
			else
			{
				std::cout << "The node already exists in the tree" << std::endl;
				return;
			}
		}
		Node* z = new Node;
		z->info = key;
		z->left = sentinel;
		z->right = sentinel;
		z->height = 1;
		z->parent = y;
		if (y == sentinel)
		{
			root = z;
		}
		else
		{
			if (z->info < y->info)
			{
				y->left = z;
			}
			else
			{
				y->right = z;
			}
		}
		INSERT_REPARE(z->parent);
	}

	void INSERT_REPARE(Node* x)
	{
		while (x != sentinel)
		{
			if (BF(x) == 0)
			{
				return;
			}
			if (BF(x) == -2)
			{
				if (BF(x->left) == -1)
				{
					x = RIGHT_ROTATION(x);
				}
				else if (BF(x->left) == 1)
				{
					LEFT_ROTATION(x->left);
					x = RIGHT_ROTATION(x);
				}
			}
			else if (BF(x) == 2)
			{
				if (BF(x->right) == -1)
				{
					RIGHT_ROTATION(x->right);
					x = LEFT_ROTATION(x);
				}
				else if (BF(x->right) == 1)
				{
					x = LEFT_ROTATION(x);
				}
			}
			x->height = 1 + std::max(x->left->height, x->right->height);
			x = x->parent;
		}
	}

	Node* MAXIMUM(Node* x)
	{
		if (x == sentinel)
		{
			return sentinel;
		}
		Node* y = x;
		while (y->right != sentinel)
		{
			y = y->right;
		}
		return y;
	}

	Node* MINIMUM(Node* x)
	{
		if (x == sentinel)
		{
			return sentinel;
		}
		Node* y = x;
		while (y->left != sentinel)
		{
			y = y->left;
		}
		return y;
	}

	Node* SUCCESSOR(Node* x)
	{
		if (FIND(x->info) == sentinel)
		{
			return sentinel;
		}
		Node* y = sentinel;
		if (x->right != sentinel)
		{
			y = MINIMUM(x->right);
		}
		else
		{
			y = x->parent;
			while (y != sentinel && x == y->right)
			{
				x = y;
				y = y->parent;
			}
		}
		return y;
	}

	Node* PREDECESSOR(Node* x)
	{
		Node* node = FIND(x->info);
		if (node == sentinel)
		{
			return sentinel;
		}
		Node* y = sentinel;
		if (x->left != sentinel)
		{
			y = MAXIMUM(x->left);
		}
		else
		{
			y = x->parent;
			while (y != sentinel && x == y->left)
			{
				x = y;
				y = y->parent;
			}
		}
		return y;
	}

	Node* FIND(int key)
	{
		Node* currentnode = root;
		while (currentnode != sentinel)
		{
			if (currentnode->info == key)
			{
				return currentnode;
			}
			else if (key < currentnode->info)
			{
				currentnode = currentnode->left;
			}
			else
			{
				currentnode = currentnode->right;
			}
		}
		return sentinel;
	}

	Node* TRANSPLANT(Node* u, Node* v)
	{
		if (u->parent == sentinel)
		{
			root = v;
		}
		else
		{
			if (u == u->parent->left)
			{
				u->parent->left = v;
			}
			else
			{
				u->parent->right = v;
			}
		}
		if (v != sentinel)
		{
			v->parent = u->parent;
		}
		return u;
	}

	void DELETE(int key)
	{
		Node* node = FIND(key);
		if (node == sentinel)
		{
			return;
		}
		ERASE(node);
	}

	void ERASE(Node* x)
	{
		if (x == sentinel)
		{
			return;
		}
		Node* parent = x->parent;
		if (x->left == sentinel && x->right == sentinel)
		{
			if (parent == sentinel)
			{
				root = sentinel;
			}
			else if (x == parent->left)
			{
				parent->left = sentinel;
			}
			else
			{
				parent->right = sentinel;
			}
			delete x;
			DELETE_REPARE(parent);
			return;
		}
		else if (x->left == sentinel || x->right == sentinel)
		{
			Node* child = sentinel;
			if (x->left != sentinel)
			{
				child = x->left;
			}
			else
			{
				child = x->right;
			}
			Node* parent = x->parent;
			if (parent == sentinel)
			{
				root = child;
			}
			else if (x == parent->left)
			{
				parent->left = child;
			}
			else
			{
				parent->right = child;
			}
			child->parent = parent;
			DELETE_REPARE(child->parent);
			delete x;
		}
		else
		{
			Node* successor = SUCCESSOR(x);
			x->info = successor->info;
			ERASE(successor);
		}
	}

	void DELETE_REPARE(Node* x)
	{
		while (x != sentinel)
		{
			int balanceFactor = BF(x);
			if (balanceFactor == 2)
			{
				if (BF(x->right) == -1)
				{
					RIGHT_ROTATION(x->right);
					x = LEFT_ROTATION(x);
				}
				else if (BF(x->right) == 0 || BF(x->right) == 1)
				{
					x = LEFT_ROTATION(x);
				}
			}
			else if (balanceFactor == -2)
			{
				if (BF(x->left) == 1)
				{
					LEFT_ROTATION(x->left);
					x = RIGHT_ROTATION(x);
				}
				else if (BF(x->left) == -1 || BF(x->left) == 0)
				{
					x = RIGHT_ROTATION(x);
				}
			}
			if (x->left == sentinel && x->right->height == 1)
			{
				Node* v = x->right;
				TRANSPLANT(x, v);
				x = v;
			}
			else if (x->right == sentinel && x->left->height == 1)
			{
				Node* v = x->left;
				TRANSPLANT(x, v);
				x = v;
			}
			x->height = 1 + std::max(x->left->height, x->right->height);
			Node* parent = x->parent;
			while (parent != sentinel)
			{
				parent->height = 1 + std::max(parent->left->height, parent->right->height);
				parent = parent->parent;
			}
			x = x->parent;
		}
	}

	Node* LEFT_ROTATION(Node* x)
	{
		Node* y;
		y = x->right;
		x->right = y->left;
		if (y->left != sentinel)
		{
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == sentinel)
		{
			root = y;
		}
		else
		{
			if (x == x->parent->left)
			{
				x->parent->left = y;
			}
			else
			{
				x->parent->right = y;
			}
		}
		y->left = x;
		x->parent = y;
		x->height = 1 + std::max(x->left->height, x->right->height);
		y->height = 1 + std::max(y->left->height, y->right->height);
		return y;
	}

	Node* RIGHT_ROTATION(Node* x)
	{
		Node* y;
		y = x->left;
		x->left = y->right;
		if (y->right != sentinel)
		{
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == sentinel)
		{
			root = y;
		}
		else
		{
			if (x == x->parent->right)
			{
				x->parent->right = y;
			}
			else
			{
				x->parent->left = y;
			}
		}
		y->right = x;
		x->parent = y;
		x->height = 1 + std::max(x->left->height, x->right->height);
		y->height = 1 + std::max(y->left->height, y->right->height);
		return y;
	}

	void PRINT_TREE_PREORDER(Node* node)
	{
		if (node == sentinel)
		{
			return;
		}
		else
		{
			std::cout << node->info << " bf = " << BF(node) << " ";
			PRINT_TREE_PREORDER(node->left);
			PRINT_TREE_PREORDER(node->right);
		}
	}

	void PRINT_TREE_INORDER(Node* node)
	{
		if (node == sentinel)
		{
			return;
		}
		else
		{
			PRINT_TREE_INORDER(node->left);
			std::cout << node->info << " bf = " << BF(node) << " ";
			PRINT_TREE_INORDER(node->right);
		}
	}

	void PRINT_TREE_POSTORDER(Node* node)
	{
		if (node == sentinel)
		{
			return;
		}
		else
		{
			PRINT_TREE_POSTORDER(node->left);
			PRINT_TREE_POSTORDER(node->right);
			std::cout << node->info << " bf = " << BF(node) << " ";
		}
	}

	void PRINT_TREE_ON_LEVELS(Node* node)
	{
		if (node == sentinel)
		{
			return;
		}
		std::queue<Node*> nodes;
		nodes.push(node);
		while (!nodes.empty())
		{
			int numberOfNodesOnLevel = nodes.size();
			for (int i = 0; i < numberOfNodesOnLevel; i++)
			{
				Node* currentNode = nodes.front();
				nodes.pop();
				std::cout << currentNode->info << " bf =  " << BF(currentNode) << " ";
				if (currentNode->left != sentinel)
				{
					nodes.push(currentNode->left);
				}
				if (currentNode->right != sentinel)
				{
					nodes.push(currentNode->right);
				}
			}
			std::cout << std::endl;
		}
	}

	void PRINT_TREE(int opt)
	{
		if (opt == 1)
		{
			PRINT_TREE_PREORDER(root);
			std::cout << std::endl;
		}
		else if (opt == 2)
		{
			PRINT_TREE_INORDER(root);
			std::cout << std::endl;
		}
		else if (opt == 3)
		{
			PRINT_TREE_POSTORDER(root);
			std::cout << std::endl;
		}
		else if (opt == 4)
		{
			PRINT_TREE_ON_LEVELS(root);
		}
	}

	void CLEAR()
	{
		if (root == sentinel)
		{
			return;
		}
		std::queue<Node*>nodes;
		nodes.push(root);
		while (!nodes.empty())
		{
			Node* currentNode = nodes.front();
			nodes.pop();
			if (currentNode->left != sentinel)
			{
				nodes.push(currentNode->left);
			}
			if (currentNode->right != sentinel)
			{
				nodes.push(currentNode->right);
			}
			delete currentNode;
		}
		root = sentinel;
	}

	bool EMPTY()
	{
		if (root == sentinel)
		{
			return true;
		}
		return false;
	}

	void CONSTRUCT(std::vector<int> keys)
	{
		CLEAR();
		if (keys.empty())
		{
			return;
		}
		for (int i = 0; i < keys.size(); i++)
		{
			INSERT(keys[i]);
		}
	}
};

void printOptions()
{
	std::cout << "Options: " << std::endl;
	std::cout << "1 - INSERT" << std::endl;
	std::cout << "2 - CONSTRUCT" << std::endl;
	std::cout << "3 - FIND" << std::endl;
	std::cout << "4 - DELETE" << std::endl;
	std::cout << "5 - MAXIMUM" << std::endl;
	std::cout << "6 - MINIMUM" << std::endl;
	std::cout << "7 - PRINT_TREE" << std::endl;
	std::cout << "8 - CLEAR" << std::endl;
	std::cout << "9 - EXIT" << std::endl << std::endl;
}

void menu()
{
	AVL AVLtree;
	int answer, value;
	int numberOfNodes, key;
	std::vector<int>keys;
	Node* node;
	while (true)
	{
		printOptions();
		std::cout << "Choose an option: ";
		std::cin >> answer;
		std::cout << std::endl;
		switch (answer)
		{
		case 1:
			std::cout << "Introduce the value of the node: ";
			std::cin >> value;
			AVLtree.INSERT(value);
			std::cout << std::endl;
			break;
		case 2:
			std::cout << "Number of keys: ";
			std::cin >> numberOfNodes;
			if (numberOfNodes <= 0)
			{
				break;
			}
			std::cout << "Keys: ";
			for (int i = 0; i < numberOfNodes; i++)
			{
				std::cin >> key;
				keys.push_back(key);
			}
			AVLtree.CONSTRUCT(keys);
			keys.clear();
			std::cout << std::endl;
			break;
		case 3:
			std::cout << "Introduce the value of the node you want to find: ";
			std::cin >> value;
			node = AVLtree.FIND(value);
			if (node != AVLtree.sentinel)
			{
				std::cout << "Node that has the key " << value << " does exist in the tree" << std::endl;
			}
			else if (node == AVLtree.sentinel)
			{
				std::cout << "Node that has the key " << value << " does not exist in the tree" << std::endl;
			}
			std::cout << std::endl;
			break;
		case 4:
			std::cout << "Introduce the value of the node you want to delete: ";
			std::cin >> value;
			AVLtree.DELETE(value);
			std::cout << std::endl;
			break;
		case 5:
			node = AVLtree.MAXIMUM(AVLtree.root);
			if (node != AVLtree.sentinel)
			{
				std::cout << "The key of the maximum node is " << node->info << std::endl;
			}
			else
			{
				std::cout << "The tree is empty" << std::endl;
			}
			std::cout << std::endl;
			break;
		case 6:
			node = AVLtree.MINIMUM(AVLtree.root);
			if (node != AVLtree.sentinel)
			{
				std::cout << "The key of the minimum node is " << node->info << std::endl;
			}
			else
			{
				std::cout << "The tree is empty" << std::endl;
			}
			std::cout << std::endl;
			break;
		case 7:
			std::cout << "Choose an option of printing the AVL tree:" << std::endl;
			std::cout << "   1 - Preorder" << std::endl;
			std::cout << "   2 - Inorder" << std::endl;
			std::cout << "   3 - Postorder" << std::endl;
			std::cout << "   4 - Levels" << std::endl;
			std::cin >> value;
			AVLtree.PRINT_TREE(value);
			std::cout << std::endl;
			break;
		case 8:
			AVLtree.CLEAR();
			std::cout << std::endl;
			break;
		case 9:
			std::cout << std::endl;
			return;
		default: std::cout << "Choose another option: " << std::endl;
		}
	}
}

int main()
{
	menu();
	return 0;
}