#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <direct.h>
#include <cstdlib>
#include <conio.h>

using namespace std;

struct Autobus
{
	int num;
	char fio[100];
	int route;
};

struct Autopark
{
	Autobus aut;
	Autopark* next;
	Autopark* prev;
};

struct Routes
{
	Autobus aut;
	Routes* next;
	Routes* prev;
};

struct Node {
	Autobus info;
	Node* next;
	Node* prev;
};

struct NodeList {
	Node* head = NULL;
	Node* tail = NULL;

	void addLast(Autobus i) {
		Node* node = new Node;
		node->info = i;
		node->next = NULL;
		if (head == NULL) {
			tail = node; head = node;
		}
		else {
			node->prev = tail;
			tail->next = node;
			tail = node;
		}
	}

	void addFirst(Autobus i) {
		Node* node = new Node;
		node->info = i;
		node->next = NULL;
		if (head == NULL) {
			head = node; tail = node;
		}
		else {
			node->next = head;
			head->prev = node;
			head = node;
		}
	}

	Node* find(int key)
	{
		Node* cur = head;
		while (cur)
		{
			if (cur->info.num == key)break;
			cur = cur->next;
		}
		return cur;
	}

	Node* insert(int key, Autobus ins)
	{
		if (Node * pkey = find(key)) {
			Node* cur = new Node;
			cur->info = ins;
			cur->next = pkey->next;
			cur->prev = pkey;
			pkey->next = cur;
			if (pkey != tail)(cur->next)->prev = cur;
			else tail = cur;
			return cur;
		}
		return 0;
	}

	bool remove(int key)
	{
		if (Node * pkey = find(key)) {
			if (pkey == head) {
				head = (head)->next;
				(head)->prev = 0;
			}
			else if (pkey == tail) {
				tail = (tail)->prev;
				(tail)->next = 0;
			}
			else {
				(pkey->prev)->next = pkey->next;
				(pkey->next)->prev = pkey->prev;
			}
			delete pkey;
			return true;
		}
		return false;
	}
	void Print()
	{
		Node* cur = new Node;
		cur = head;
		while (cur) {
			cout << "Bus number: " << cur->info.num << endl;
			cout << "Fio of its driver: " << cur->info.fio << endl;
			cout << "Route number: " << cur->info.route << endl << endl;
			cur = cur->next;
		}
		return;
	}
};

void busAdder(int d)
{
	ofstream f("Buses.bin", ios::binary);
	Autobus cur;
	for (int i = 0; i < d; ++i) {
		cout << "Enter fio of the driver: ";
		cin.clear();

		cin.getline(cur.fio, 100);
		cout << "Enter number of route: ";
		cin.clear();

		cin >> cur.route;
		cin.clear();
		while (cin.get() != '\n')continue;
		cur.num = i + 1;
		f.write((char*)& cur, sizeof(Autobus));
	}
	f.close();
}

void busReader(int& count)
{
	ifstream f("Buses.bin", ios::binary);
	int cnt = 0;
	Autobus tmp;
	while (f.read((char*)& tmp, sizeof(Autobus))) {
		cnt++;
	}
	f.seekg(0);
	Autobus* mas = new Autobus[cnt];
	for (int i = 0; i < cnt; ++i) {
		f.read((char*)& mas[i], sizeof(Autobus));
	}
	f.close();
	count = cnt;
	ifstream f1("Buses.bin", ios::binary);

	for (int i = 0; i < cnt; ++i) {
		f1.read((char*)& tmp, sizeof(Autobus));
		cout << "Number: " << tmp.num << endl;
		cout << "Fio: " << tmp.fio << endl;
		cout << "Route number: " << tmp.route << endl << endl;;
	}
	f1.close();
	return;

}

void AddPark(int, Autobus*, NodeList&, NodeList&);
void AddRoute(int, Autobus*, NodeList&, NodeList&);
int menu();
bool seek(int, NodeList, NodeList);

int main()
{
	const int d = 5;

	int buscnt = 0;



	ifstream f("Buses.bin", ios::binary);
	int cnt = 0;
	Autobus tmp;
	while (f.read((char*)& tmp, sizeof(Autobus))) {
		cnt++;
	}
	f.close();



	ifstream f1("Buses.bin", ios::binary);
	Autobus* mas = new Autobus[cnt];
	for (int i = 0; i < cnt; ++i) {
		f1.read((char*)& mas[i], sizeof(Autobus));
	}
	f1.close();



	NodeList Routes, Park;

	bool isRunning = 1;
	while (isRunning) {
		int key = menu();
		switch (key) {
		case 0: AddRoute(cnt, mas, Routes, Park); break;
		case 1: AddPark(cnt, mas, Park, Routes); break;
		case 2: isRunning = 0;
		}
	}

	delete[] mas;
	return 0;
}


void AddRoute(int cnt, Autobus* mas, NodeList& Routes, NodeList& Park)
{
	cout << "Add: 1\nShow: 0\n\n";
	int t = 0;
	do {
		cin >> t;
	} while (t != 1 && t != 0);
	system("cls");
	int key = 0;
	bool flag = 0;
	bool temp = 0;
	if (t == 1) {
		do {
			//while (cin.get() != '\n')continue;
			system("cls");
			cout << "Buses:\n\n";
			for (int i = 0; i < cnt; ++i) {
				//if (!seek(mas[i].num, Routes, Park)) {
				cout << "Number: " << mas[i].num << endl;
				cout << "Fio: " << mas[i].fio << endl;
				cout << "Route number: " << mas[i].route << endl << endl;
				//}
			}
			cout << "Total: " << cnt << " buses." << endl;
			cout << "Which autobus would u like to send on route?\n";
			do {
				cin >> key;
				for (int i = 0; i < cnt; ++i) {
					if (key == mas[i].num) {
						if (!seek(key, Routes, Park)) {
							Routes.addLast(mas[i]);
						}
						else cout << "There is such bus already on the Route or in the Park\n";
						flag = true;
						break;
					}
				}
				if (!flag) {
					std::cout << "There is no bus with such number, try existing number!\n";
					system("pause");
				}
				cin.clear();
			} while (!flag);
			cout << "If it is the end, press 1, else press 0!\n";
			do {
				cin >> temp;
			} while (temp != 0 && temp != 1);
		} while (!temp);
		system("cls");
		cout << "Buses on the Route:\n\n";
		Routes.Print();
		system("pause");
	}
	else {
		cout << "Buses on the Route:\n\n";
		Routes.Print();
		system("pause");
	}
}

void AddPark(int cnt, Autobus* mas, NodeList& Park, NodeList& Routes)
{
	cout << "Add: 1\nShow: 0\n\n";
	int t = 0;
	do {
		cin >> t;
	} while (t != 1 && t != 0);
	system("cls");
	int key = 0;
	bool flag = 0;
	bool temp = 0;
	if (t == 1) {
		do {
			//while (cin.get() != '\n')continue;
			system("cls");
			cout << "Buses:\n\n";
			for (int i = 0; i < cnt; ++i) {
				if (!seek(mas[i].num, Routes, Park)) {
					cout << "Number: " << mas[i].num << endl;
					cout << "Fio: " << mas[i].fio << endl;
					cout << "Route number: " << mas[i].route << endl << endl;
				}
			}
			cout << "Total: " << cnt << " buses." << endl;
			cout << "Which autobus would u like to send on route?\n";
			do {
				cin >> key;
				for (int i = 0; i < cnt; ++i) {
					if (key == mas[i].num) {
						if (!seek(key, Routes, Park)) {
							Park.addLast(mas[i]);
						}
						else cout << "There is such bus already on the Route or in the Park\n";
						flag = true;
						break;
					}
				}
				if (!flag) {
					cout << "There is no bus with such number, try existing number!\n";
					system("pause");
				}
				cin.clear();
			} while (!flag);
			cout << "If it is the end, press 1, else press 0!\n";
			do {
				cin >> temp;
			} while (temp != 0 && temp != 1);
		} while (!temp);
		system("cls");
		cout << "Buses in the Park:\n\n";
		Park.Print();
		system("pause");
	}
	else {
		cout << "Buses in the Park:\n\n";
		Park.Print();
		system("pause");
	}
}

int menu() {
	int key = 0;
	int code;
	do {
		system("cls");
		key = (key + 3) % 3;
		if (key == 0) cout << "-> AddRoute" << endl;
		else  cout << "   AddRoute" << endl;
		if (key == 1) cout << "-> AddPark" << endl;
		else  cout << "   AddPark" << endl;
		if (key == 2) cout << "-> Exit" << endl;
		else  cout << "   Exit" << endl;
		code = _getch();
		if (code == 224)
		{
			code = _getch();
			if (code == 80) key++;
			if (code == 72) key--;
		}
	} while (code != 13);
	system("cls");
	return key;
}

bool seek(int key, NodeList Routes, NodeList Park)
{
	Node* cur = Routes.head;
	while (cur) {
		if (cur->info.num == key)return 1;
		cur = cur->next;
	}
	cur = Park.head;
	while (cur) {
		if (cur->info.num == key)return 1;
		cur = cur->next;
	}

	return 0;
}