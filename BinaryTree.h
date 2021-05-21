#include <iostream>
#include <string>
#include <queue>

using namespace std;

class BinaryTree
{
protected:
	//Структура для хранения данных(имени, даты рождения, оценки)
	struct Data
	{
		string name;
		int dateOfBirth = 0, mark = 0;
		//Инициализация через конструктор
		Data(string name, int dateOfBirth, int mark)
		{
			this->name = name;
			this->dateOfBirth = dateOfBirth;
			this->mark = mark;
		}
		Data(){}
		//Метод, возвращающий все поля структуры в виде строки
		string toString()
		{

			return "Name: " + name + ". Mark: " + to_string(mark) + ". Date of birth: " + ParseDate();
		}
		//Парсим переданную дату в строку для более понятного вывода
		string ParseDate()
		{
			string res = "";
			bool isYear = true;
			int date = dateOfBirth;
			while (date > 0)
			{
				//Деление нацело на 100 даст остаток в виде двузначного числа(года, месяца, дня)
				int remainder = date % 100;
				//В зависимости от последних двух цифр года, определяем 4х значный год
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
				//добавляем нолик, если месяц/день меньше 10
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
	//Вершина(узел) дерева
	struct Node
	{
		Data data;
		//Указатели на левую и правую вершину
		Node* left = NULL;
		Node* right = NULL;

	};
	//Корень дерева
	Node* root = NULL;
	
	//Проверка на количество введённых символов в имени
	bool CheckInput(string name)
	{
		return name.length() >= 3;
	}

	//Проверка, введено ли 6 чисел в дате рождения
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

	//Удаление дерева с памяти(Обход дерева аналогичен методу вывода)
	//Note: Используется деструктором
	void removeTree()
	{
		if (root == NULL) return;
		queue<Node*> nodesQueue;

		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//Если левый узел существует, пушим его в очередь, затем обнуляем
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
				curNode->left = NULL;
			}
			//Аналогично для правого узла
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
				curNode->right = NULL;
			}
			curNode = NULL;
		}
		//Обнуляем корень дерева
		root = NULL;
	}
	//Для сравнения дат суммируем число, месяц умноженный на 30 дней, и год на 365
	int DateToNum(int* date)
	{
		return date[0] + date[1] * 30 + date[2] * 365;
	}

	//Возвращает true, если переданная дата больше текущей
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

	//Добавление элемента в дерево, сортировка по дате рождения.
	///Note: Используется только для "пересыпки" дерева, для пользователя не доступна.
	bool AddByBirth(Data data)
	{

		//Создание нового узла
		Node* newNode = new Node;
		newNode->data = data;

		//Если корень дерева "пустой"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//В противном случае получаем указатель на корень дерева
		Node* curNode = root;
		while (true)
		{
			//если переданная оценка меньше текущей, отправляемся в левую ветку
			if (!isLaterPassedDate(curNode->data.dateOfBirth, newNode->data.dateOfBirth))
			{
				//Если левого элемента не существует, добавляем его в дерево
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//переходим на левый узел
				curNode = curNode->left;
			}
			//В противном случае аналогично проходим правую ветку
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
	//Деструктор удаляет дерево с памяти при завершении работы с программой
	~BinaryTree()
	{
		removeTree();
		cout << "Tree removed from destructor\n";
	}

	//Добавление элемента в дерево, сортировка по оценке
	bool Add(string name, int dateOfBirth, int mark)
	{
		//Если переданные данные не валидны, выходим из метода
		if (!CheckInput(name) || !CheckInput(dateOfBirth)) return false;
		//Создание нового узла
		Node* newNode = new Node;
		newNode->data = Data(name, dateOfBirth, mark);
		//Если корень дерева "пустой"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//В противном случае получаем указатель на корень дерева
		Node* curNode = root;
		while (true)
		{
			//если переданная оценка меньше текущей, отправляемся в левую ветку
			if (newNode->data.mark < curNode->data.mark)
			{
				//Если левого элемента не существует, добавляем его в дерево
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//переходим на левый узел
				curNode = curNode->left;
			}
			//В противном случае аналогично проходим правую ветку
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

	//Вывод дерева
	void ShowTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty";
			return;
		}
		//Очередь для узлов
		queue<Node*> nodesQueue;

		//Пушим корень дерева
		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			//Указатель на текущий узел получаем с очереди
			Node* curNode = nodesQueue.front();
			//Выкидываем текущий элемент очереди
			nodesQueue.pop();

			cout << curNode->data.toString() << endl;
			//Если есть левый или правый узел дерева, добавляем его в очередь, пока она не станет пуста
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
	
	//Поиск средней оценки среди всех(аналогичный метод - вывод дерева)
	float FindAverageMark()
	{
		int sum = 0, counter = 0;
		queue<Node*> nodesQueue;

		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//Суммирование оценок и подсчёт количества узлов дерева
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

	//Возвращает объект класса BinaryTree, в который "пересыпаны" сортировкой по дате рождения элементы основного класса.
	//Удаляет дерево основного класса. Выводит результат "пересыпанного" дерева.
	void CopyTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty, nothing to copy";
			return;
		}
		//Создаём новый класс бинарного дерева
		BinaryTree bt;

		queue<Node*> nodesQueue;

		nodesQueue.push(root);
		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//Добавление в дерево(сортировка по дате рождения)
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
		//Инициализация через конструктор
		Data(string name, int age, int experience, long number, float salary)
		{
			this->name = name;
			this->age = age;
			this->experience = experience;
			this->salary = salary;
			this->number = number;
		}
		Data() {}
		//Метод, возвращающий все поля структуры в виде строки
		string toString()
		{

			return "Name: " + name + ". Age: " + to_string(age) + ". Experience: " + to_string(experience) + ". Number: " + to_string(number) + ". Salary: " + to_string(salary);
		}
	};

	struct Node
	{
		Data data;
		//Указатели на левую и правую вершину
		Node* left = NULL;
		Node* right = NULL;

	};

	Node* root;

	bool AddByExperience(Data data)
	{

		//Создание нового узла
		Node* newNode = new Node;
		newNode->data = data;

		//Если корень дерева "пустой"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//В противном случае получаем указатель на корень дерева
		Node* curNode = root;
		while (true)
		{
			//если переданная оценка меньше текущей, отправляемся в левую ветку
			if (curNode->data.experience > newNode->data.experience)
			{
				//Если левого элемента не существует, добавляем его в дерево
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//переходим на левый узел
				curNode = curNode->left;
			}
			//В противном случае аналогично проходим правую ветку
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

			//Если левый узел существует, пушим его в очередь, затем обнуляем
			if (curNode->left != NULL)
			{
				nodesQueue.push(curNode->left);
				curNode->left = NULL;
			}
			//Аналогично для правого узла
			if (curNode->right != NULL)
			{
				nodesQueue.push(curNode->right);
				curNode->right = NULL;
			}
			curNode = NULL;
		}
		//Обнуляем корень дерева
		root = NULL;
	}
	
public:

	bool Add(string name, int age, int experience, long number, float salary)
	{
		//Создание нового узла
		Node* newNode = new Node;
		newNode->data = Data(name, age, experience, number, salary);
		//Если корень дерева "пустой"
		if (root == NULL)
		{
			root = newNode;
			return true;
		}
		//В противном случае получаем указатель на корень дерева
		Node* curNode = root;
		while (true)
		{
			//если переданный номер меньше текущего, отправляемся в левую ветку
			if (newNode->data.number < curNode->data.number)
			{
				//Если левого элемента не существует, добавляем его в дерево
				if (curNode->left == NULL)
				{
					curNode->left = newNode;
					return true;
				}
				//переходим на левый узел
				curNode = curNode->left;
			}
			//В противном случае аналогично проходим правую ветку
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

			//Суммирование оценок и подсчёт количества узлов дерева
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

	//Вывод дерева
	void ShowTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty";
			return;
		}
		//Очередь для узлов
		queue<Node*> nodesQueue;

		//Пушим корень дерева
		nodesQueue.push(root);

		while (!nodesQueue.empty())
		{
			//Указатель на текущий узел получаем с очереди
			Node* curNode = nodesQueue.front();
			//Выкидываем текущий элемент очереди
			nodesQueue.pop();

			cout << curNode->data.toString() << endl;
			//Если есть левый или правый узел дерева, добавляем его в очередь, пока она не станет пуста
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

	//Выводит пересыпанное дерев сортировкой по стажу.
	//Удаляет дерево основного класса.
	void CopyTree()
	{
		if (root == NULL)
		{
			cout << "Tree is empty, nothing to copy";
			return;
		}
		//Создаём новый класс бинарного дерева
		VarBT bt;

		queue<Node*> nodesQueue;

		nodesQueue.push(root);
		while (!nodesQueue.empty())
		{
			Node* curNode = nodesQueue.front();
			nodesQueue.pop();

			//Добавление в дерево(сортировка по дате рождения)
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