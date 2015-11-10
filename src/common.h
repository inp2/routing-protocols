#include <cstdio>
#include <vector>

using namespace std;

typedef vector<pair<int, int> > Table;
struct Message {
		int src;
		int dest;
		string content;
};

void print_table(vector<Table> &tables, set<int> &nodes)
{
		for (int src: nodes) {
				cerr << "Table for node " << src << endl;
				Table &table = tables[src];
				for (int dest : nodes) { 
						auto &entry = table[dest];
						cout << dest << " " << entry.first << " " << entry.second << endl;
				}
		}
}

void send_messages(vector<Table> &tables, vector<Message> &msg_list)
{ 
		for (Message &msg: msg_list) {
				cout << "from " << msg.src << " to " << msg.dest << " hops ";
				int cur = msg.src;
				string hops;
				while (cur != msg.dest) {
						cout << cur << " ";
						Table &table = tables[cur]; 
						int next = table[msg.dest].first;
						cur = next; 
				}
				cout << msg.content << endl;
		}
}
