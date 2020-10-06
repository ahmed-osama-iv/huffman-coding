#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Node{

    char character = '\0';
    int frq;

    Node *left = NULL, *right = NULL;

};

struct comp{
	bool operator()(Node* l, Node* r){
		// highest priority item has lowest frequency
		return l->frq > r->frq;
	}
};



class Huffman{

    public:

        static string encode(string text){

            init();

            for(char c : text){
                frq[c]++;
            }

            buildTree();

            string binaryString = "";
            for(char c : text){
                binaryString += huffmanCode[c];
            }

            int cnt = 0;
            while(binaryString.size() % 8 != 0){
                cnt++;
                binaryString = '0' + binaryString;
            }

            string encodedString = "";
            for(int i = 0; i < binaryString.size(); i += 8){
                int c = 0;
                for(int j = 0; j < 8; j++){
                    c += ((binaryString[i+j] - '0') << j);
                }
                encodedString += (char)c;
            }

            return "HM" + to_string(cnt) + " " + header() + encodedString;
        }

        static string decode(string encodedString){

            init();

            stringstream ss(encodedString);
            string head;
            int lines_counter;

            ss >> head >> lines_counter;

            if(head.size() < 3) return "ERROR";
            if(head[0] != 'H' || head[1] != 'M' || head[2] > '9'|| head[2] < '0')  return "ERROR";
            start = (head[2] - '0');

            string line;

            getline(ss, line);

            while(lines_counter--){

                getline(ss, line);

                if(line[1] == '-') {
                    frq['\n'] = stoi(line.substr(2, line.size()-2));
                }else{
                    frq[line[0]] = stoi(line.substr(2, line.size()-2));
                }
            }

            buildTree();

            encodedString = "";

            while(getline(ss, line)){
                if(encodedString == "") encodedString = line;
                else encodedString += "\n" + line;
            }
            return decode_string(encodedString);
        }



    private :

        static Node* root;
        static map <char, string> huffmanCode;
        static map <char, int> frq;
        static priority_queue<Node*, vector<Node*>, comp> pq;
        static int start;


        static void init(){
            root = NULL;
            huffmanCode.clear();
            frq.clear();
            pq = *new priority_queue<Node*, vector<Node*>, comp>;
        }

        static void buildTree(){

            for(auto p : frq){
                pq.push(new Node{p.first, p.second});
            }

            while(pq.size()>1){

                Node *top1 = pq.top(); pq.pop();
                Node *top2 = pq.top(); pq.pop();

                root = new Node{'\0', top1->frq + top2->frq, top1, top2};
                pq.push(root);
            }

            dfs(*root);

        }

        static void dfs(Node u, string s = ""){

            if(u.left)
                dfs(*u.left, s + '0');

            if(u.right)
                dfs(*u.right, s + '1');

            if(!u.left && !u.right){
                huffmanCode[u.character] = s;
            }
        }

        static string header(){

            string s;
            s += to_string(frq.size()) + "\n";
            for(auto p : frq){

                if(p.first == '\n'){
                    s += "--" + to_string(p.second) + "\n";
                }

                else{
                    s += p.first;
                    s += " " + to_string(p.second) + "\n";
                }
            }

            return s;
        }

        static string decode_string(string encodedString){

            string binaryString = "";

            for(char c : encodedString){

                for(int i=0; i<8; i++){
                    if((c >> i) & 1) binaryString += '1';
                    else binaryString += '0';
                }
            }


            string text = "";

            Node *curNode = root;
            for(int index = start; index < binaryString.size(); index++){
                if(binaryString[index] == '0') curNode = curNode->left;
                else curNode = curNode->right;

                if(curNode->character){
                    text += curNode->character;
                    curNode = root;
                }
            }

            return text;
        }

};

Node* Huffman::root;
map <char, string> Huffman::huffmanCode;
map <char, int> Huffman::frq;
priority_queue<Node*, vector<Node*>, comp> Huffman::pq;
int Huffman::start;

void encodeFile(string in, string out){
    ifstream inFile;
    inFile.open(in);
    string line, text;
    while(getline(inFile, line)){
        if(text == "") text = line;
        else text += '\n' + line;
    }


    ofstream outFile;
    outFile.open(out);
    outFile << Huffman::encode(text);
}


void decodeFile(string in, string out){
    ifstream inFile;
    inFile.open(in);
    string line, text;
    while(getline(inFile, line)){
        if(text == "") text = line;
        else text += '\n' + line;
    }

    ofstream outFile;
    outFile.open(out);
    outFile << Huffman::decode(text);


}

int main()
{

    encodeFile("input.txt", "kk.txt");
    decodeFile("kk.txt", "pp.txt");
}



