In file included from src/linkstate.c:14:0:
src/common.h:4:1: error: ‘use’ does not name a type
 use namespace std;
 ^
src/common.h:6:9: error: ‘vector’ does not name a type
 typedef vector<pair<int, int> > Table;
         ^
src/common.h:10:3: error: ‘string’ does not name a type
   string content;
   ^
src/common.h:11:24: error: ‘string’ does not name a type
   void show_hops(const string &hops) {
                        ^
src/common.h:11:32: error: ISO C++ forbids declaration of ‘hops’ with no type [-fpermissive]
   void show_hops(const string &hops) {
                                ^
src/common.h: In member function ‘void Message::show_hops(const int&)’:
src/common.h:13:26: error: ‘content’ was not declared in this scope
         src, dest, hops, content);
                          ^
src/common.h: At global scope:
src/common.h:18:20: error: variable or field ‘send_messages’ declared void
 void send_messages(vector<Table> &tables, vector<Message> &msg_list)
                    ^
src/common.h:18:20: error: ‘vector’ was not declared in this scope
src/common.h:18:20: note: suggested alternative:
In file included from /usr/include/c++/4.8/vector:64:0,
                 from /usr/include/c++/4.8/bits/random.h:34,
                 from /usr/include/c++/4.8/random:50,
                 from /usr/include/c++/4.8/bits/stl_algo.h:65,
                 from /usr/include/c++/4.8/algorithm:62,
                 from src/linkstate.c:6:
/usr/include/c++/4.8/bits/stl_vector.h:210:11: note:   ‘std::vector’
     class vector : protected _Vector_base<_Tp, _Alloc>
           ^
In file included from src/linkstate.c:14:0:
src/common.h:18:27: error: ‘Table’ was not declared in this scope
 void send_messages(vector<Table> &tables, vector<Message> &msg_list)
                           ^
src/common.h:18:35: error: ‘tables’ was not declared in this scope
 void send_messages(vector<Table> &tables, vector<Message> &msg_list)
                                   ^
src/common.h:18:43: error: ‘vector’ was not declared in this scope
 void send_messages(vector<Table> &tables, vector<Message> &msg_list)
                                           ^
src/common.h:18:43: note: suggested alternative:
In file included from /usr/include/c++/4.8/vector:64:0,
                 from /usr/include/c++/4.8/bits/random.h:34,
                 from /usr/include/c++/4.8/random:50,
                 from /usr/include/c++/4.8/bits/stl_algo.h:65,
                 from /usr/include/c++/4.8/algorithm:62,
                 from src/linkstate.c:6:
/usr/include/c++/4.8/bits/stl_vector.h:210:11: note:   ‘std::vector’
     class vector : protected _Vector_base<_Tp, _Alloc>
           ^
In file included from src/linkstate.c:14:0:
src/common.h:18:57: error: expected primary-expression before ‘>’ token
 void send_messages(vector<Table> &tables, vector<Message> &msg_list)
                                                         ^
src/common.h:18:60: error: ‘msg_list’ was not declared in this scope
 void send_messages(vector<Table> &tables, vector<Message> &msg_list)
                                                            ^
src/linkstate.c:116:6: warning: unused parameter ‘msg_list’ [-Wunused-parameter]
 void parse_msg(std::string line, std::vector<int> & msg_list)
      ^
make: *** [obj/linkstate.o] Error 1
