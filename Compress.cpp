#include <iostream>
#include <bitset>
#include <string>
#include <queue> 
#include <map>
#include <fstream>
#include <streambuf>

using namespace std;

class transformer
{
public:
	transformer(){}
	bitset <5>* compress (string input);
	string decompress(bitset<5>*input);
};


struct Node 
{
	int weight;
	char rep;
	Node* left;
	Node* right;
	Node()
	{
		left=NULL;
		right=NULL;
		rep = 0;
		weight=0;
	}
	Node *merge(Node *input)
	{
		Node *output= new Node;
		output->weight= this->weight + input->weight;
		output->left = this;
		output->right = input;
		return output;
	}

};

struct cmp
{
   bool operator()( const Node* a, const Node* b ) const 
   {
    return a->weight > b->weight;
   }
};
class transhuffer
{
private:
	int characters;
	Node* tree;
	map < char, string> encoding;
public:
	transhuffer(){}
	void compress();
	void decompress();
};
void encode(Node* tree, string &mapping, map <char, string> &map  );




int main()
{
	transhuffer a;
	a.compress();
	a.decompress();
	return 0;
}

bitset <5>* transformer::compress (string input)
	{
		int x = input.length();
		bitset<5>*cmp =  new bitset<5>[x+1];
		for (int i = 0; i < x; ++i)
		{
			bitset<5> buff(input[i]-97);
			cmp[i]=buff;
		}
		cmp[x]=-1;
		return cmp;

	}
string transformer::decompress(bitset<5>*input)
{
	char output[100]=" ";
	for (int i = 0; input[i]!=-1; ++i)
	{
		int num = input[i].to_ulong()+97;
		output[i]=num;
	}
	return output;


}

void transhuffer::compress()
{
	ifstream instream("sample.txt");
  	string input( (istreambuf_iterator<char>(instream) ),
                       (istreambuf_iterator<char>()    ) );
	int len= input.length();
	int pos=0;
	priority_queue<Node*, vector<Node*>, cmp> queue;
	Node** array= new Node*[256];
	for (int i = 0; i < 256; i++)
        array[i] = new Node;
	bool found = false;
	for (int i = 0; i < len; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			if (array[j]->rep==input[i])
			{
				(array[j]->weight)++;
				found=true;
				break;
			}
			
		}
		if (found==false&& input[i]>0)
			{
				array[pos]->weight=1;
				array[pos]->rep=input[i];
				pos++;
			}
		found= false;

	}
	for (int i = 0; i < pos; ++i)
	{
		queue.push(array[i]);
	}
	tree = new Node;
	while(queue.size()!=1)
	{
		Node* a= queue.top();
		queue.pop();
		Node* b= queue.top();
		queue.pop();
		tree = a->merge(b);
		queue.push(tree);
	}
	tree = queue.top();
	Node* newtree = tree;
	string mapping="";
	map < char, string> map;
	encode(newtree, mapping, map);
	encoding=map;
	string bitform = "";
	for (int i = 0; i < input.length(); ++i)
	{
		bitform=bitform + map[input[i]];
	}
	string output="";
	int spare=0;
	characters=0;
	for (int i = 0; i+8 < bitform.length(); i+=8)
	{
		bitset<8> temp(bitform.substr(i,8));
		char buff=temp.to_ulong();
		output= output+buff;
		spare=i;
		characters++;
	}
	if(bitform.length()%8!=0)
	{
		output=output + bitform.substr(spare+8);
	}
	ofstream outstream;
	outstream.open("compressed.txt");
	outstream << output;


}

void encode(Node* tree, string &mapping, map <char, string> &map  )
{
	
	if(tree->rep > 0)
	{
		map[tree->rep]=mapping;
	}
	if(tree->left!=NULL)
	{
		mapping= mapping + '0';
		encode(tree->left, mapping, map);
		
	}
	if(tree->right!=NULL)
	{
		mapping= mapping + '1';
		encode(tree->right, mapping, map);

	}
	mapping.erase(mapping.end()-1);
	return;
	


}

void transhuffer::decompress()
{
	ifstream instream("compressed.txt");
  	string input( (istreambuf_iterator<char>(instream) ),
                       (istreambuf_iterator<char>()    ) );
  	int pos=0;
  	string encoding="";
  	for (int i = 0; i< characters; ++i)
  	{
  		bitset<8> temp(input[i]);
  		encoding=encoding + temp.to_string();
  		pos=i;
  	}
  	encoding= encoding + input.substr(pos+1);
  	Node* start = tree;
  	string output="";
  	for (int i = 0; i < encoding.length(); ++i)
  	{
  		if (encoding[i]=='1')
  		{
  			tree= tree->right;
  		}
  		if (encoding[i]=='0')
  		{
  			tree= tree->left;
  		}
  		if(tree->rep!=0)
  		{
  			output= output+ tree->rep;
  			tree= start;

  		}
  	}
  	ofstream outstream;
  	outstream.open("decompressed.txt");
  	outstream << output;
}