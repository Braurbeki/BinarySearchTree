#include <iostream>
#include <string>
#include <queue>

using namespace std;

class BinaryTree
{
protected:
	//��������� ��� �������� ������(�����, ���� ��������, ������)
	struct Data
	{
		string name;
		int dateOfBirth = 0, mark = 0;
		//������������� ����� �����������
		Data(string name, int dateOfBirth, int mark)
		{
			this->name = name;
			this->dateOfBirth = dateOfBirth;
			this->mark = mark;
		}
		Data(){}
		//�����, ������������ ��� ���� ��������� � ���� ������
		string toString()
		{

			return "Name: " + name + ". Mark: " + to_string(mark) + ". Date of birth: " + ParseDate();
		}
		//������ ���������� ���� � ������ ��� ����� ��������� ������
		string ParseDate()
		{
			string res = "";
			bool isYear = true;
			int date = dateOfBirth;
			while (date > 0)
			{
				//������� ������ �� 100 ���� ������� � ���� ����������� �����(����, ������, ���)
				int remainder = date % 100;
				//� ����������� �� ��������� ���� ���� ����, ���������� 4� ������� ���
				if (isYear)
				{
					if (remainder < 10)
					{
						res += "200";
					}
					else if (remainder > 21)
					{
						res += "19";
					}
					else
					{
						res += "20";
					}

					isYear = false;
				}
				//��������� �����, ���� �����/���� ������ 10
				else if (remainder < 10)
				{
					res += "0";
				}

				res += to_string(remainder) + "/";

				date /= 100;
			}
			return res;
		}
	};
	//�������(����) ������
	struct Node
	{
		Data data;
		//��������� �� ����� � ������ �������
		Node* left = NULL;
		Node* right = NULL;

	};
	//������ ������
	Node* root = NULL;
	
	//�������� �� ���������� �������� �������� � �����
	bool CheckInput(string name)
	{
		return name.length() >= 3;
	}

	//��������, ������� �� 6 ����� � ���� ��������
	bool CheckInput(int dateOfBirth)
	{
		int counter = 0;
		while (dateOfBirth > 0)
		{
			counter++;
			dateOfBirth /= 10;
		}
		return counter == 6 || counter == 5;
	}

	//�������� ������ � ������(����� ������ ���������� ������ ������)
	//Note: ������������ ������������
	void removeTree()
	{
		if (root == NULL) return;
		queue<Node*> nodesQueue;

		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//���� ����� ���� ����������, ����� ��� � �������, ����� ��������
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
				curNode->left = NULL;
			}
			//���������� ��� ������� ����
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
				curNode->right = NULL;
			}
			curNode = NULL;
		}
		//�������� ������ ������
		root = NULL;
	}
	//��� ��������� ��� ��������� �����, ����� ���������� �� 30 ����, � ��� �� 365
	int DateToNum(int* date)
	{
		return date[0] + date[1] * 30 + date[2] * 365;
	}

	//���������� true, ���� ���������� ���� ������ �������
	bool isLaterPassedDate(int curDate, int passedDate)
	{
		int resCurDate[3], resPassedDate[3];
		int i = 2;
		while (curDate > 0 || passedDate > 0)
		{
			resCurDate[i] = curDate % 100;
			resPassedDate[i] = passedDate % 100;
			curDate /= 100;
			passedDate /= 100;
			i--;
		}

		return DateToNum(resCurDate) < DateToNum(resPassedDate);
	}

	//���������� �������� � ������, ���������� �� ���� ��������.
	///Note: ������������ ������ ��� "���������" ������, ��� ������������ �� ��������.
	bool AddByBirth(Data data)
	{

		//�������� ������ ����
		Node* newNode = new Node;
		newNode->data = data;

		//���� ������ ������ "������"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//� ��������� ������ �������� ��������� �� ������ ������
		Node* curNode = root;
		while (true)
		{
			//���� ���������� ������ ������ �������, ������������ � ����� �����
			if (!isLaterPassedDate(curNode->data.dateOfBirth, newNode->data.dateOfBirth))
			{
				//���� ������ �������� �� ����������, ��������� ��� � ������
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//��������� �� ����� ����
				curNode = curNode->left;
			}
			//� ��������� ������ ���������� �������� ������ �����
			else
			{
				if (curNode->right == NULL)
				{
					curNode->right = newNode;
					return true;
				}

				curNode = curNode->right;
			}
		}
	}

public:
	//���������� ������� ������ � ������ ��� ���������� ������ � ����������
	~BinaryTree()
	{
		removeTree();
		cout << "Tree removed from destructor\n";
	}

	//���������� �������� � ������, ���������� �� ������
	bool Add(string name, int dateOfBirth, int mark)
	{
		//���� ���������� ������ �� �������, ������� �� ������
		if (!CheckInput(name) || !CheckInput(dateOfBirth)) return false;
		//�������� ������ ����
		Node* newNode = new Node;
		newNode->data = Data(name, dateOfBirth, mark);
		//���� ������ ������ "������"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//� ��������� ������ �������� ��������� �� ������ ������
		Node* curNode = root;
		while (true)
		{
			//���� ���������� ������ ������ �������, ������������ � ����� �����
			if (newNode->data.mark < curNode->data.mark)
			{
				//���� ������ �������� �� ����������, ��������� ��� � ������
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//��������� �� ����� ����
				curNode = curNode->left;
			}
			//� ��������� ������ ���������� �������� ������ �����
			else
			{
				if (curNode->right == NULL)
				{
					curNode->right = newNode;
					return true;
				}

				curNode = curNode->right;
			}
		}
	}

	//����� ������
	void ShowTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty";
			return;
		}
		//������� ��� �����
		queue<Node*> nodesQueue;

		//����� ������ ������
		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			//��������� �� ������� ���� �������� � �������
			Node* curNode = nodesQueue.front();
			//���������� ������� ������� �������
			nodesQueue.pop();

			cout << curNode->data.toString() << endl;
			//���� ���� ����� ��� ������ ���� ������, ��������� ��� � �������, ���� ��� �� ������ �����
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
			}
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
			}
		}
	}
	
	//����� ������� ������ ����� ����(����������� ����� - ����� ������)
	float FindAverageMark()
	{
		int sum = 0, counter = 0;
		queue<Node*> nodesQueue;

		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//������������ ������ � ������� ���������� ����� ������
			sum += curNode->data.mark;
			counter++;

			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
			}
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
			}
		}

		return (float) sum / counter;
	}

	//���������� ������ ������ BinaryTree, � ������� "����������" ����������� �� ���� �������� �������� ��������� ������.
	//������� ������ ��������� ������. ������� ��������� "�������������" ������.
	void CopyTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty, nothing to copy";
			return;
		}
		//������ ����� ����� ��������� ������
		BinaryTree bt;

		queue<Node*> nodesQueue;

		nodesQueue.push(root);
		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//���������� � ������(���������� �� ���� ��������)
			bt.AddByBirth(curNode->data);
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
			}
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
			}
		}
		bt.ShowTree();
		removeTree();
	}
};


class VarBT: public BinaryTree
{

	struct Data
	{
		string name;
		int age = 0, experience = 0;
		long number = 0l;
		float salary = 0;
		//������������� ����� �����������
		Data(string name, int age, int experience, long number, float salary)
		{
			this->name = name;
			this->age = age;
			this->experience = experience;
			this->salary = salary;
			this->number = number;
		}
		Data() {}
		//�����, ������������ ��� ���� ��������� � ���� ������
		string toString()
		{

			return "Name: " + name + ". Age: " + to_string(age) + ". Experience: " + to_string(experience) + ". Number: " + to_string(number) + ". Salary: " + to_string(salary);
		}
	};

	struct Node
	{
		Data data;
		//��������� �� ����� � ������ �������
		Node* left = NULL;
		Node* right = NULL;

	};

	Node* root;

	bool AddByExperience(Data data)
	{

		//�������� ������ ����
		Node* newNode = new Node;
		newNode->data = data;

		//���� ������ ������ "������"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//� ��������� ������ �������� ��������� �� ������ ������
		Node* curNode = root;
		while (true)
		{
			//���� ���������� ������ ������ �������, ������������ � ����� �����
			if (curNode->data.experience > newNode->data.experience)
			{
				//���� ������ �������� �� ����������, ��������� ��� � ������
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//��������� �� ����� ����
				curNode = curNode->left;
			}
			//� ��������� ������ ���������� �������� ������ �����
			else
			{
				if (curNode->right == NULL)
				{
					curNode->right = newNode;
					return true;
				}

				curNode = curNode->right;
			}
		}
	}

	void removeTree()
	{
		if (root == NULL) return;
		queue<Node*> nodesQueue;

		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//���� ����� ���� ����������, ����� ��� � �������, ����� ��������
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
				curNode->left = NULL;
			}
			//���������� ��� ������� ����
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
				curNode->right = NULL;
			}
			curNode = NULL;
		}
		//�������� ������ ������
		root = NULL;
	}
	
public:

	bool Add(string name, int age, int experience, long number, float salary)
	{
		//�������� ������ ����
		Node* newNode = new Node;
		newNode->data = Data(name, age, experience, number, salary);
		//���� ������ ������ "������"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//� ��������� ������ �������� ��������� �� ������ ������
		Node* curNode = root;
		while (true)
		{
			//���� ���������� ����� ������ ��������, ������������ � ����� �����
			if (newNode->data.number < curNode->data.number)
			{
				//���� ������ �������� �� ����������, ��������� ��� � ������
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//��������� �� ����� ����
				curNode = curNode->left;
			}
			//� ��������� ������ ���������� �������� ������ �����
			else
			{
				if (curNode->right == NULL)
				{
					curNode->right = newNode;
					return true;
				}

				curNode = curNode->right;
			}
		}
	}

	float FindAverageSalary()
	{
		float sum = 0, counter = 0;
		queue<Node*> nodesQueue;

		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//������������ ������ � ������� ���������� ����� ������
			sum += curNode->data.salary;
			counter++;

			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
			}
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
			}
		}

		return (float)sum / counter;
	}

	//����� ������
	void ShowTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty";
			return;
		}
		//������� ��� �����
		queue<Node*> nodesQueue;

		//����� ������ ������
		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			//��������� �� ������� ���� �������� � �������
			Node* curNode = nodesQueue.front();
			//���������� ������� ������� �������
			nodesQueue.pop();

			cout << curNode->data.toString() << endl;
			//���� ���� ����� ��� ������ ���� ������, ��������� ��� � �������, ���� ��� �� ������ �����
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
			}
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
			}
		}
	}

	//������� ������������ ����� ����������� �� �����.
	//������� ������ ��������� ������.
	void CopyTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty, nothing to copy";
			return;
		}
		//������ ����� ����� ��������� ������
		VarBT bt;

		queue<Node*> nodesQueue;

		nodesQueue.push(root);
		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//���������� � ������(���������� �� ���� ��������)
			bt.AddByExperience(curNode->data);
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
			}
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
			}
		}
		bt.ShowTree();
		removeTree();
	}
};