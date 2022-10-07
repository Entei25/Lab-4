#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


vector <vector <int>> next_matrix;
vector <vector <int>> distance_matrix;

template <typename T>


struct Graph {
private:
	vector <T> nodes;
public:


	T get_node(int i) {
		return nodes[i];
	}
	
	void resize_nodes(T node) 
	{
		for (int i = 0; i < nodes.size(); i++)
		{	
			int control = (nodes.size()) - nodes[i].size();
			for (int j = 0; j < (control); j++)
			{
				nodes[i].insert_edge(INT_MAX);
			}
		}
	}

	 int find_index(string name) {
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].get_name() == name)
			{
				return i;
			}

		}
		return -1;
		
	 }
	
	 bool find(string name)
	 {
		
		 for (int i = 0; i < nodes.size(); i++)
		 {
			 if (nodes[i].get_name() == name)
			 {
				 return true;
			 }

		 }
		 return false;
	 }


	void push_node(T node) {
		nodes.push_back(node);
		resize_nodes(node);
	}
	
	int size() {
		return nodes.size();
	} 
	
	void add_edge(int i, int j, int weight) {
		nodes[i].modificate_edge(j,weight);
		nodes[j].modificate_edge(i,weight);
	}

	void modificate_edge(int i, int j, int weight) {
		nodes[i].modificate_edge(j, weight);
		nodes[j].modificate_edge(i, weight);
	}
	
	void delete_edge(int i, int j) {
		nodes[i].modificate_edge(j, INT_MAX);
		nodes[j].modificate_edge(i, INT_MAX);
	}

	void delete_node(int name) {
		nodes.erase(nodes.begin() + name);
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i].delete_edge(name);
		}

	}

};



class Enrut
{
private:
	vector <int> cost;
	string name;
public:


	Enrut(string name) {
		this->name = name;
	}

	string get_name() {
		return name;
	}

	int size()
	{
		return cost.size();
	}


	int get_edge_cost(int cost_value)
	{
		return cost[cost_value];
	}


	void insert_edge(int cost_node)
	{
		cost.push_back(cost_node);
	}

	void modificate_edge(int node, int cost_node)
	{
		cost[node] = cost_node;
	}

	void delete_edge(int node)
	{
		cost[node] = INT_MAX;
	}

};



//get adjacency matrix
vector <vector <int>> create_adjacency_matrix(Graph <Enrut> net)
{
	vector <vector <int>> matrix;
	for (int i = 0; i < net.size(); ++i)
	{
		vector <int> row;
		for (int j = 0; j < net.size(); ++j)
		{
			if (i == j)
			{
				row.push_back(0);
			}
			else
			{
				row.push_back(INT_MAX);
			}
		}
		matrix.push_back(row);
	}
	for (int i = 0; i < net.size(); ++i)
	{
		for (int j = 0; j < net.size(); ++j)
		{
			if (i != j)
			{
				matrix[i][j] = net.get_node(i).get_edge_cost(j);
			}
		}
	}
	//print matrix
	/*for (int i = 0; i < net.size(); ++i)
	{
		for (int j = 0; j < net.size(); ++j)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}*/
	
	

	return matrix;
}


//floyd warshall algorithm

void floyd_warshall(Graph <Enrut> net)
{
	vector <vector <int>> matrix = create_adjacency_matrix(net);
	next_matrix.resize(matrix.size(), vector<int>(matrix.size()));
	distance_matrix.resize(matrix.size(), vector<int>(matrix.size(), INT_MAX));

	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			distance_matrix[i][j] = matrix[i][j];
			if (matrix[i][j] != INT_MAX && i != j)
			{
				next_matrix[i][j] = j;
				
			}
			else
			{
				next_matrix[i][j] = -1;
			}
			next_matrix[i][i] = i;
		}
	}
	
		
	
	for (int k = 0; k < matrix.size(); k++)
	{
		for (int i = 0; i < matrix.size(); i++)
		{
			for (int j = 0; j < matrix.size(); j++)
			{
				if (distance_matrix[i][k] != INT_MAX && distance_matrix[k][j] != INT_MAX && distance_matrix[i][k] + distance_matrix[k][j] < distance_matrix[i][j])
				{
					distance_matrix[i][j] = distance_matrix[i][k] + distance_matrix[k][j];
					next_matrix[i][j] = next_matrix[i][k];
				}
			}
		}
	}

	
	// print shortest path matrix
	/*for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix.size(); j++) {
			cout << distance_matrix[i][j] << " ";
		}
		cout << endl;
	}
	*/
	// for printing path


	
	

	
}

vector <int> get_path(int u, int v)
{
	vector <int> path;
	path.push_back(u);
	while (u != v)
	{
		u = next_matrix[u][v];
		path.push_back(u);
	}


	return path;
}




pair <vector <string>,vector <vector <int>>> from_txt(string in)
{
	pair <vector <string>, vector <vector <int>>> data;
	int count = 0;
	vector <vector <int> > matrix;
	vector <string> names;
	ifstream input(in);
	while (!input.eof())
	{
		count++;

		string s;
		getline(input, s);
		if (count == 1)
		{
			for (int i = 0; i < s.length(); i++)
			{
				if (s[i] == ' ' || s[i] == s.length()-1)
				{
					string name = "";
					name += (s[i] == s.length() - 1) ? s[i] : s[i - 1];
					names.push_back(name);
				}
			}
		}
		else
		{
			vector <int> row;
			for (int i = 0; i < s.length(); i++)
			{
				if (s[i] == ' ' || i == s.length() - 1)
				{
					string s1 = "";
					s1 += (i == s.length() - 1) ? s[i] : (s[i - 1]);
					int a = 0;
					if (s1[0] == '-')
					{
						a = INT_MAX;
					}
					else
					{
						a = stoi(s1);

					}
					row.push_back(a);
				}
			}
			matrix.push_back(row);
		}


	}
	
	data.first = names;
	data.second = matrix;
	
	return data;
	

}





int main()
{
	Graph <Enrut> Net;
	
	int option;
	


	cout << "que deseas realizar" << endl;
	cout << "1.insertar un enrutador" << endl;
	cout << "2.eliminar un enrutador" << endl;
	cout << "3.modificar un enrutador" << endl;
	cout << "4.hacer un enlace entre enrutadores" << endl;
	cout << "5.eliminar un enlace entre enrutadores" << endl;
	cout << "6.ver costo de envio de un enrutador a otro" << endl;
	cout << "7.ver camino de envio de un enrutador a otro" << endl;
	cout << "8.cargar una red desde un archivo" << endl;
	cout << "0.salir" << endl;
	cin >> option;
	while (option != 0)
	{

		switch (option)
		{
		case 1:
		{
			string name;
			cout << "ingrese el nombre del enrutador" << endl;
			cin >> name;
			if (Net.find(name) == false)
			{
				Enrut node(name);
				Net.push_node(node);
				break;
				cout << endl;
				cout << endl;
			}
			else
			{
				cout << "-->" << "el enrutador ya existe" << endl;
				break;
				cout << endl;
				cout << endl;
			}
		}
		case 2:
		{
			string name;
			cout << "ingrese el nombre del enrutador" << endl;
			cin >> name;
			if (Net.find(name) == true)
			{
				int name_del = Net.find_index(name);
				Net.delete_node(name_del);
				break;
				cout << endl;
				cout << endl;
			}
			else
			{
				cout << "-->" << "el enrutador no existe" << endl;
				break;
				cout << endl;
				cout << endl;
			}
		}
		case 3:
		{
			string name;
			cout << "ingrese el nombre del enrutador" << endl;
			cin >> name;
			cout << "a que nodo quieres cambiar el costo" << endl;
			string name2;
			cin >> name2;
			int name_del = Net.find_index(name);
			int name_del2 = Net.find_index(name2);
			cout << "ingrese el nuevo costo" << endl;
			int cost;
			cin >> cost;
			floyd_warshall(Net);
			Net.modificate_edge(name_del, name_del2, cost);
			break;
			cout << endl;
			cout << endl;
		}
		case 4:
		{
			string name1;
			string name2;
			int cost;
			cout << "ingrese el nombre del enrutador 1" << endl;
			cin >> name1;
			cout << "ingrese el nombre del enrutador 2" << endl;
			cin >> name2;
			cout << "ingrese el costo del enlace" << endl;
			cin >> cost;
			floyd_warshall(Net);
			int name_del1 = Net.find_index(name1);
			int name_del2 = Net.find_index(name2);
			Net.add_edge(name_del1, name_del2, cost);
			cout << endl;
			cout << endl;
			break;
		}
		case 5:
		{
			string name1;
			string name2;
			cout << "ingrese el nombre del enrutador 1" << endl;
			cin >> name1;
			cout << "ingrese el nombre del enrutador 2" << endl;
			cin >> name2;
			floyd_warshall(Net);
			int name_del1 = Net.find_index(name1);
			int name_del2 = Net.find_index(name2);
			Net.delete_edge(name_del1, name_del2);
			cout << endl;
			cout << endl;
			break;
		}
		case 6:
		{
			string name1;
			string name2;
			cout << "ingrese el nombre del enrutador 1" << endl;
			cin >> name1;
			cout << "ingrese el nombre del enrutador 2" << endl;
			cin >> name2;
			floyd_warshall(Net);
			int a = Net.find_index(name1);
			int b = Net.find_index(name2);
			(distance_matrix[a][b] == INT_MAX) ? cout << "-->" << "no hay un enlace entre " << name1 << " y " << name2 << endl : cout << "-->" << "el costo de envio es: " << distance_matrix[a][b] << endl;
			cout << endl;
			cout << endl;
			break;
		}
		case 7:
		{
			string name1;
			string name2;
			cout << "ingrese el nombre del enrutador 1" << endl;
			cin >> name1;
			cout << "ingrese el nombre del enrutador 2" << endl;
			cin >> name2;
			int a = Net.find_index(name1);
			int b = Net.find_index(name2);
			floyd_warshall(Net);
			vector <int> path = get_path(a, b);
			for (int i = 0; i < path.size(); i++)
			{
				cout << "-->" << Net.get_node(path[i]).get_name() << " ";
			}
			cout << endl;
			cout << endl;
			break;

		}
		case 8:
		{
			pair <vector <string>, vector <vector <int>>> import = from_txt("input.txt");
			for (int i = 0; i < import.first.size(); i++)
			{
				Net.push_node(import.first[i]);
			}
			for (int i = 0; i < import.second.size(); i++)
			{
				for (int j = 0; j < import.second[i].size(); j++)
				{
					Net.add_edge(i, j, import.second[i][j]);
				}
			}
			cout << endl;
			cout << endl;
		}

		}
		cout << endl;
		cout << endl;
		cout << "que deseas realizar" << endl;
		cout << "1.insertar un enrutador" << endl;
		cout << "2.eliminar un enrutador" << endl;
		cout << "3.modificar un enrutador" << endl;
		cout << "4.hacer un enlace entre enrutadores" << endl;
		cout << "5.eliminar un enlace entre enrutadores" << endl;
		cout << "6.ver costo de envio de un enrutador a otro" << endl;
		cout << "7.ver camino de envio de un enrutador a otro" << endl;
		cout << "8.cargar una red desde un archivo" << endl;
		cout << "0.salir" << endl;
		cin >> option;
		
		
	}
}
