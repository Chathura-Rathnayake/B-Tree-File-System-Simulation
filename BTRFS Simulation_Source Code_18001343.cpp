#include <bits/stdc++.h> 
#include <iostream>
#include <ctime>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <vector>

#define KEYS 9
 
using namespace std; 

class BTreeNode  //A class for nodes
{ 
private:
	string data[KEYS]; //an array of strings (only 8byte strings are allocated to these array)
	int min_degree;	 
	int filled_key_amount;	
	int L_bit; // Is leaf? (the leaf bit)  
	int *keys_array; 
	BTreeNode **child_pointer_array; 

	
public: 

	BTreeNode(int m_d, int l_b); // minimumm degree , leaf bit
	BTreeNode *search(int val); //if key value is not present function returns null
	void Deletion(int val); 
	void non_filled_node_insertion(int val, string str);  
	void merge(int index);
	void split(int pos, BTreeNode *node);  
	int FindTheKey(int val); 
	void traverse(); 
	friend void copyFile();
	friend int main();
	friend void modifyFile();
	friend void viewFile();
friend class BTree; 

}; 

class BTree //A class for trees
{
 
private:
	
	BTreeNode *root; 
	int min_degree; 
	
public: 
	
	BTree(int m_d) // Constructor 
	{ 
		min_degree = m_d; 
		root = NULL; 
	
	} 
	void traverse() 
	{ 
		if (root != NULL) 
			{
				root->traverse(); 
			}
	}

	BTreeNode* search(int val)
	{ 
		if(root == NULL)
		{
			return NULL;
		}
		else
		{
			return (root->search(val));
		}

	} 

	void insert(int val, string str); 
	void Deletion(int val); 

}; 

BTreeNode::BTreeNode(int m_d, int l_b) //constructor
{ 

//initializing data members of node

	filled_key_amount = 0; 
	min_degree = m_d; 
	L_bit = l_b;
	keys_array = new int[2*min_degree-1]; 
	child_pointer_array = new BTreeNode *[2*min_degree]; 

} 

//used to find the index of the first key that is greater than or equal to a certain key value
int BTreeNode::FindTheKey(int val) 
{ 
	int index=0; 
	while ((index<filled_key_amount) && (keys_array[index] < val))
	{
		index=index+1; 
	}
	return (index); 
} 

//used to traverse through the whole tree
void BTreeNode::traverse() 
{ 
	int i; 
	for (i = 0; i < filled_key_amount; i++) 
	{ 
		if (L_bit == 0) //if it is not a leaf
		{
			child_pointer_array[i]->traverse(); //traverse recursively
		} 
		cout<<keys_array[i]<<"-"<<data[i]<<endl; 
		
	} 


	if (L_bit == 0)
	{
		child_pointer_array[i]->traverse(); 
	}
	
} 

//used to merge two nodes
void BTreeNode::merge(int index) 
{ 
	BTreeNode *child1 = child_pointer_array[index]; //address of the node1 to merge
	BTreeNode *child2 = child_pointer_array[index+1]; //address of the node2 to merge


	child1->keys_array[min_degree-1] = keys_array[index]; 
	child1->data[min_degree-1] = data[index]; 

	for (int i=0; i<child2->filled_key_amount; ++i) //copying child2's keys to child1 and also strings
	{
		child1->keys_array[i+min_degree] = child2->keys_array[i]; 
		child1->data[i+min_degree] = child2->data[i]; 
	}

	if (!child1->L_bit) //copying child2's child pointers to child1
	{ 
		for(int i=0; i<=child2->filled_key_amount; ++i)
		{
			child1->child_pointer_array[i+min_degree] = child2->child_pointer_array[i]; 
		}		
	} 

	for (int i=index+1; i<filled_key_amount; ++i) 
	{
		keys_array[i-1] = keys_array[i]; 
		data[i-1] = data[i]; 	
	} 

	for (int i=index+2; i<=filled_key_amount; ++i)
    {
		child_pointer_array[i-1] = child_pointer_array[i]; 
	}

	child1->filled_key_amount += child2->filled_key_amount+1; 
	filled_key_amount--; 

	delete(child2); 
} 

//used to insert a key to the tree
void BTree::insert(int val, string str) 
{ 

	if (root == NULL) //initial situation (empty tree)
	{ 
		root = new BTreeNode(min_degree, 1); 
		root->filled_key_amount = 1; //just one key
		root->keys_array[0] = val;  //key value
		(root->data[0]=str); //inserting the 5byte string to root
	} 
	else 
	{ 
		int temp= (root->filled_key_amount == 2*min_degree-1); 
		
		if (temp) //situation where the root is fully occupied by keys
		{ 
		
			BTreeNode *newRoot = new BTreeNode(min_degree,0); //new root node
			newRoot->child_pointer_array[0] = root; 
			newRoot->split(0, root); 

			//adding the newly inserted key to the tree	
			int i = 0; 
			if (newRoot->keys_array[0] < val) //traverse to the position
			{
				i=i+1;
			}	 
			newRoot->child_pointer_array[i]->non_filled_node_insertion(val,str); //insert the key 
		
			
			root = newRoot; 
		} 
		else //situation where the root is not full
		{
			root->non_filled_node_insertion(val,str); //just add the key
		}
			
	} 
} 
//used to split a node
void BTreeNode::split(int pos, BTreeNode *node) 
{ 
	//initialize the new node
	BTreeNode *newNode = new BTreeNode(node->min_degree, node->L_bit); 
	newNode->filled_key_amount = min_degree - 1; 

	//copy necessary key values to the new node
	for (int i = 0; i < min_degree-1; i++)
	{
		newNode->keys_array[i] = node->keys_array[i+min_degree]; 
		newNode->data[i]=node->data[i+min_degree]; 
	}
	//copy necessary children addresses
	if (node->L_bit == false) 
	{ 
		for (int i = 0; i < min_degree; i++)
		{
			newNode->child_pointer_array[i] = node->child_pointer_array[i+min_degree]; 
		}			
	} 

	node->filled_key_amount = min_degree - 1; //update number of keys in old node

	for (int i = filled_key_amount; i >= pos+1; i--) 
	{
		child_pointer_array[i+1] = child_pointer_array[i]; 
	}
		
	child_pointer_array[pos+1] = newNode; //create the link between two nodes
	
	for (int i = filled_key_amount-1; i >= pos; i--)
	{
		keys_array[i+1] = keys_array[i]; 	
	}
	
	keys_array[pos] = node->keys_array[min_degree-1]; 
	data[pos]=node->data[min_degree-1]; 
	filled_key_amount++; 
} 

//used to insert a value to a non filled node (not fully filled)
void BTreeNode::non_filled_node_insertion(int val, string str) 
{ 
	int rmtIndex = filled_key_amount-1;  //get the rightmost element's index

	if (L_bit) //when current node is a leaf node
	{ 
		while (rmtIndex >= 0 && keys_array[rmtIndex] > val) //all key values those are greater than val is moved one position up in the key array
		{ 
			keys_array[rmtIndex+1] = keys_array[rmtIndex]; 
			data[rmtIndex+1]=data[rmtIndex]; 
			rmtIndex--; 
		} 

		keys_array[rmtIndex+1] = val; //insert the key element to suitable position
		data[rmtIndex+1]=str; //insert the string to suitable position
		filled_key_amount = filled_key_amount+1; 
	} 
	else  //when current node is a non leaf node
	{ 

		while (rmtIndex >= 0 && keys_array[rmtIndex] > val) //search the child who is going to get the new key
		{
			rmtIndex--; 
		}
	
		if (child_pointer_array[rmtIndex+1]->filled_key_amount == 2*min_degree-1) //if that child is full
		{ 
			split(rmtIndex+1, child_pointer_array[rmtIndex+1]); //split the node
					
			if (keys_array[rmtIndex+1] < val) 
			{
				rmtIndex++; 	
			}		
		} 
		child_pointer_array[rmtIndex+1]->non_filled_node_insertion(val,str);  
	} 
} 

//used to search a key and returns the address of the particular node it belongs
BTreeNode *BTreeNode::search(int val) 
{ 
	
	int i = 0; 
	while (i < filled_key_amount && val > keys_array[i])
	{
		i++; 
	}
	
	if (keys_array[i] == val) //key is found and exit
	{
		return this; 
	}
	
	if (L_bit == 1) //key is not found and exit
	{
		return NULL; 
	}
		
	return child_pointer_array[i]->search(val); //keep searching
}

//used to delete a key  (utility)
void BTreeNode::Deletion(int val) 
{ 
	int index = FindTheKey(val); 

	if ((index < filled_key_amount) && (keys_array[index] == val)) //key is in current node
	{ 
		if (L_bit==1) //if current node is a leaf
		{	
			int i;
			for (i=index+1; i<filled_key_amount; i++) //keys those are located after the index is moved one position back
			{
				keys_array[i-1] = keys_array[i]; 
				data[i-1]=data[i]; 
			}
			
			filled_key_amount=filled_key_amount-1;  //update the key count
		}
		else //if current node is not a leaf
		{ 
			int val = keys_array[index]; 
			
			if ((child_pointer_array[index+1]->filled_key_amount) >= min_degree) //leftmost
			{ 	
				BTreeNode *current = child_pointer_array[index+1]; 
				//get the left most value of the tree
				while (!current->L_bit) 
				{
					current = current->child_pointer_array[0]; 
				}
				int s=current->keys_array[0]; //add the first key of the leaf node to s	
				string s1=current->data[0]; 
				data[index]=s1; //replacing
				keys_array[index] = s; 
				child_pointer_array[index+1]->Deletion(s); //recursively delete
			}
			else if ((child_pointer_array[index]->filled_key_amount) >= min_degree) //rightmost
			{ 			
				BTreeNode *current=child_pointer_array[index]; 
				//get the right most value of the tree
				while (!current->L_bit)
				{
					current = current->child_pointer_array[current->filled_key_amount]; 
				}	
				int p= current->keys_array[current->filled_key_amount-1]; //add the first key of the leaf node to p		
				string s2=current->data[current->filled_key_amount-1];
				data[index]=s2;	
				keys_array[index]=p; //replacing
				child_pointer_array[index]->Deletion(p); //recursively delete
			}  
			else //both of the children has less than min_degree keys
			{ 
				merge(index); 
				child_pointer_array[index]->Deletion(val); 
			} 			
		}			
	} 
	else //key is not in the current node
	{ 
		if (L_bit==1) //if this is a leaf
		{ 
			cout<<"key is not present in the tree"<<endl; 
			return; 
		}
		 
		//if this is not a leaf
		int flag;
		if (index==filled_key_amount)
		{
			flag=1;
		}
		else
		{
			flag=0;
		}
				
		if ((child_pointer_array[index]->filled_key_amount) < min_degree) //child has less than minimumm keys
		{
			
			//fill the child
			if (index!=0 && (child_pointer_array[index-1]->filled_key_amount)>=min_degree)
			{
				//borrow a key from previous child
				BTreeNode *child2=child_pointer_array[index-1]; 
				BTreeNode *child1=child_pointer_array[index]; 
	
				int i;
				for (i=child1->filled_key_amount-1; i>=0; i-=1) 
				{
					child1->keys_array[i+1] = child1->keys_array[i]; 
					child1->data[i+1] = child1->data[i]; 
				}

				if (!child1->L_bit) 
				{ 
					for(i=child1->filled_key_amount; i>=0; i-=1)
					{
						child1->child_pointer_array[i+1] = child1->child_pointer_array[i]; 
					}			
				}
				child1->keys_array[0] = keys_array[index-1]; 
				if(!child1->L_bit)
				{
					child1->child_pointer_array[0] = child2->child_pointer_array[child2->filled_key_amount]; 
				} 	

				keys_array[index-1] = child2->keys_array[child2->filled_key_amount-1]; 
				data[index-1] = child2->data[child2->filled_key_amount-1]; 

				child2->filled_key_amount--; 
				child1->filled_key_amount++; 		
			}
		
		
			else if (index!=filled_key_amount && child_pointer_array[index+1]->filled_key_amount>=min_degree) 
			{
				//borrow a key from next child
				BTreeNode *child2=child_pointer_array[index+1]; 
				BTreeNode *child1=child_pointer_array[index]; 
				
				child1->keys_array[(child1->filled_key_amount)] = keys_array[index]; 
				child1->data[(child1->filled_key_amount)] = data[index]; 
							
				if (!(child1->L_bit))
				{
					child1->child_pointer_array[(child1->filled_key_amount)+1] = child2->child_pointer_array[0]; 
				}
				keys_array[index] = child2->keys_array[0]; 
				data[index] = child2->data[0]; 
				for (int i=1; i<child2->filled_key_amount; ++i)
				{
					child2->keys_array[i-1] = child2->keys_array[i]; 
					child2->data[i-1] = child2->data[i]; 
				}
				if (!child2->L_bit) 
				{ 
					for(int i=1; i<=child2->filled_key_amount; ++i)
					{
						child2->child_pointer_array[i-1] = child2->child_pointer_array[i]; 
					} 			
				} 			
				child2->filled_key_amount --; 
				child1->filled_key_amount ++;							
			}			
			else
			{ 
				if (index != filled_key_amount)
				{
					merge(index); 
				}				
				else
				{
					merge(index-1); 
				}		
			} 	 
		}		
		if ((flag && index) <= filled_key_amount)
		{
			child_pointer_array[index]->Deletion(val); 
		}		
		else
		{
			child_pointer_array[index-1]->Deletion(val); 
		}			
	} 
} 

//used to delete a key  (main)
void BTree::Deletion(int val) 
{ 
	if (!root) 
	{ 
		cout << "empty tree"<<endl; 
		return; 
	} 
	root->Deletion(val); 
	 
	if (root->filled_key_amount==0) 
	{ 
		BTreeNode *temp = root;  
		if (root->L_bit)
		{
				root = NULL; 
		}
		else
			root = root->child_pointer_array[0]; 
				
		delete temp; 
	} 
} 

struct metaNode //to keep metadata of files
{
	int ID; //starting ID of the file 
	int numOfBlocks; //number of blocks needed to store the file
	string name;
	int size;
	long int checksum;
	bool modified_bit;

};

static long int id_val=0; //initialize the static variable 

vector<metaNode> metaList; 

struct CoW_list_Node //CoW list (contains the updated file's data)
{
	string modifiedFile;
	int ID;
	long int checksum;
};

vector<CoW_list_Node> CoW_List_Disk1; // disk 1 CoW list
vector<CoW_list_Node> CoW_List_Disk2; // disk 2 CoW list

BTree disk_1(5); //simulates the disk 1
BTree disk_2(5); //simulates the disk 2


//to create a text file and store it on Disks (B Trees)
void createFile()
{
	/* algo
	
	1.get the user input to a string
	2.update metadata vector
	3.store it block by block in both trees (RAID level 1)
	
	*/
	
	//get the input
	string name;
	while(1) //repeat until we get a valid name from the user
	{	
		int flag=0;

		cout<<"Enter the file name: ";
		cin.ignore();
		getline(cin, name);
		 //there cannot be two files with the same name
		//search metadata vector for validity of the name
		int var;
		for (int i=0;i<metaList.size();i++)
		{		
			var=name.compare(metaList[i].name); 
		
			if(!var)
			{
				cout<<endl<<"invalid name! The name you entered is already in use. please enter another name. "<<endl<<endl;
				flag=1;
				break;
			}
		}
	
		if(!flag) //to check whether we need to repeat or not
		{
			break;
		}
	}
	
	string text;
	cout<<endl<<" -------- Enter the text file content-------- "<<endl<<endl;
	getline(cin, text);
	//update metadata vector
	id_val++;
	struct metaNode temp;
	temp.ID=id_val;
	temp.modified_bit=0;
	temp.name=name;
	int i=0,c_sum=0;
	while(text[i]) //calculate check sum
	{	
		c_sum+=text[i];
		i++;
	}	
	temp.checksum=c_sum;
	double num_of_blocks=ceil(i/8.0); //to get the number of blocks needed
	temp.numOfBlocks=num_of_blocks;
	temp.size=num_of_blocks*8;

	metaList.push_back(temp); 
	
	//store the file in both B trees (RAID level 1)
	
	int remaining_characters=i%8; //get the charaters of the last block
	string subStr;
	int index=0;
	//to split the text string to b byte sized substrings
	for(int i=1;i<num_of_blocks;i++) 
	{		
		subStr = text.substr(index,8); 
		disk_1.insert(id_val,subStr); //insert the data to disk 1 
		disk_2.insert(id_val,subStr); //insert the data to disk 2 
		index+=8;
		id_val++;
	}
	subStr=text.substr(index,remaining_characters); 
	disk_1.insert(id_val,subStr); //insert the last block of data to disk 1 
	disk_2.insert(id_val,subStr); //insert the last block of data to disk 2 
}

//to modify a text file and store it.(CoW is used)
void modifyFile() 
{
	/*
		algorithm
		1.get the file name and , make modified bit 1 for that file
		2.display the file to user and input the modified file (ask to copy paste it and modify)
		3.get the modified string and store it to CoW_list vector for both disks (file id and the string)
		
		note -
			original file is there always unless you modify that file again or you delete it.			
	*/
	
	string fName,modifiedText;
	int file_index;
	int flag=0;
	while(1) //to get the file name from user
	{
		cout<<"Enter the name of the file you want to modify: ";
		cin.ignore();
		getline(cin, fName);
		
		for(int i=0;i<metaList.size();i++) //find the corresponding file
		{
			if(!((metaList[i].name).compare(fName)))
			{
				cout<<"Copy and paste this file content to your editor space and modify it. press enter to save the modified text: "<<endl<<endl;
				//get the file from the tree
				string text; //we recollect the file to this string from tree blocks
				BTreeNode *tempB;
				int val=metaList[i].ID; //val is key
				for(int h=0;h<metaList[i].numOfBlocks;h++)
				{
					tempB=disk_1.search(val);
					for(int l=0;l<tempB->filled_key_amount;l++)
					{	
							if(tempB->keys_array[l]==val)
							{
								text+=tempB->data[l];
						   		break;			
							}
					}
					val++;
				}
				cout<<"Content Of the File: "<<text<<endl<<endl; 
							
				getline(cin, modifiedText); //get the modified text
				metaList[i].modified_bit=1; //update the modified bit in metadata list
				
				//pushing the modified file and its ID to Cow list vector
				struct CoW_list_Node temp;
				temp.ID=metaList[i].ID;
				temp.modifiedFile=modifiedText;
				int i=0,c_sum=0;
				while(modifiedText[i]) //calculate check sum
				{	
					c_sum+=modifiedText[i];
					i++;
				}
				temp.checksum=c_sum;
				
				CoW_List_Disk1.push_back(temp); //disk 1
				CoW_List_Disk2.push_back(temp); //disk 2
				flag=1;
				break;
			}	
		}
		
		if(!flag)
		{
			cout<<"File name is invalid, please try again."<<endl;
		}
		else
		{
			break;
		}
	}
}

//to list all the available files
void listFiles() 
{
	/* idea is to list file names and sizes
	
	Algorithm
		1.if modified bit==1 then
			don't print it
			print relevant data from CoW - update file id accordingly
		  print the relevant data fromm metadata vector
	*/
	
	for(int i=0;i<metaList.size();i++)
	{
		if(metaList[i].modified_bit) //if file is modified
		{
			//access the CoW list to get that particular data
			for(int j=0;j<CoW_List_Disk1.size();j++)  //iterate through CoW List
			{
				if(CoW_List_Disk1[j].ID==metaList[i].ID) //find the CoW node for relvant ID
				{
					cout<<"File Name: "<<metaList[i].name<<"	 |  ";	
					
					//calculate the size of the string
					string tempS=CoW_List_Disk1[j].modifiedFile;
					int count=0;
					while(tempS[count]){count++;} //calculate number of characters
					double num_of_blocks=ceil(count/8.0); //to get the number of blocks needed
					int size_temp=num_of_blocks*8;				
					cout<<"File Size: "<<size_temp<<" Bytes"<<endl;
					break; //loop is not needed beyond this point
				}	
			}
			continue;	//to skip this value			
		}
		cout<<"File Name: "<<metaList[i].name<<"	 |  ";
		cout<<"File Size: "<<metaList[i].size<<" Bytes"<<endl;	
	}		
}	

//to delete a text file
void deleteFile()
{
	/*
	algorithm
		1.get the file name from the user
		2.if file is modified then
			remove from both CoW Lists
		3.remove corresponding data from both b trees
		4.remove metadata.		
	*/
	string fName;
	int flag=0;
	while(1) //to get the file name from user
	{
		cout<<"Enter the name of the file you want to delete: ";
		cin.ignore();
		getline(cin, fName);

		for(int i=0;i<metaList.size();i++) //find the corresponding file
		{
			if(!((metaList[i].name).compare(fName))) //file is there.!
			{
				if(metaList[i].modified_bit) //if the file is already modified
				{
					//get the files vector index
					for (int j=0;j<CoW_List_Disk1.size();j++)  //iterate through the CoW vector
					{
						if(CoW_List_Disk1[j].ID==metaList[i].ID) //find the corresponding file's CoW index
						{		 
							CoW_List_Disk1.erase(CoW_List_Disk1.begin() + j);  //delete the record from disk 1 COW list
							CoW_List_Disk2.erase(CoW_List_Disk2.begin() + j);  //delete the record from disk 2 COW list
						}
					}					
				}
				
				//remove data from B trees			
				disk_1.Deletion(metaList[i].ID); //from disk 1
				disk_2.Deletion(metaList[i].ID); //from disk 2
				
				//remove metadata
				metaList.erase(metaList.begin() + i);
						
				flag=1;
				break;
			}	
		}
		
		if(!flag)
		{
			cout<<"File name is invalid, please try again. "<<endl;
		}
		else
		{
			break;
		}
	}	
	
}

//to create a duplicate of a file
void copyFile() 
{
	/* idea is to give the copy a different name and a different ID but same content
	
	algortihm
		1.select the correct file
		2.give a different name and id to the copy 
		3.if(file is a modified one) then
				get the CoW string and make a copy of it
				store that copy to both B Trees
				update metadata list (add new copy's information to the metadata list)
		  else
		   		get the file from B Tree and make a copy
		   		store that copy to both B Trees
		   		update metadata list (add new copy's information to the metadata list)
		
	*/
	string fName,tempName;
	int flag=0;
	while(1) //to get the file name from user
	{
		cout<<"Enter the name of the file you want to make a copy: ";
		cin.ignore();
		getline(cin, fName);

		for(int i=0;i<metaList.size();i++) //find the corresponding file
		{
			if(!((metaList[i].name).compare(fName))) //file found.!
			{
				if(metaList[i].modified_bit) //if the file is already modified
				{
					//get the file's index from CoW List
					for (int j=0;j<CoW_List_Disk1.size();j++)  //iterate through the CoW vector
					{	
						if(CoW_List_Disk1[j].ID==metaList[i].ID) //find the corresponding file's CoW index
						{	
							int temp_id=CoW_List_Disk1[j].ID;
							tempName=metaList[temp_id-1].name;												
							tempName=tempName+"_copy"; //name of that copy
							
							//inserting the file to tree
							string text=CoW_List_Disk1[j].modifiedFile; //file content
							
							//update metadata vector
							id_val++;
							struct metaNode temp;
							temp.ID=id_val;
							temp.modified_bit=0;
							temp.name=tempName;
							int i=0,c_sum=0;
							while(text[i]) //calculate check sum
							{	
								c_sum+=text[i];
								i++;
							}	
							temp.checksum=c_sum;
							double num_of_blocks=ceil(i/8.0); //to get the number of blocks needed
							temp.numOfBlocks=num_of_blocks;
							temp.size=num_of_blocks*8; 
							metaList.push_back(temp); 	
							
							//store the file in both B trees (RAID level 1)
							
							int remaining_characters=i%8; //get the charaters of the last block
							string subStr;
							int index=0;
							//to split the text string to b byte sized substrings
							for(int i=1;i<num_of_blocks;i++) 
							{		
								subStr = text.substr(index,8); 
								disk_1.insert(id_val,subStr); //insert the data to disk 1 
								disk_2.insert(id_val,subStr); //insert the data to disk 2 
								index+=8;
								id_val++;
							}
							subStr=text.substr(index,remaining_characters); 
							disk_1.insert(id_val,subStr); //insert the last block of data to disk 1 
							disk_2.insert(id_val,subStr); //insert the last block of data to disk 2 									
						}
					}
				}
				else //not a modified file
				{
							tempName=metaList[i].name;												
							tempName=tempName+"_copy"; //name of that copy
							
							//get the file from the tree
							string text; //we recollect the file to this string from tree blocks
							BTreeNode *tempB;
							int val=metaList[i].ID; //val is key
							for(int h=0;h<metaList[i].numOfBlocks;h++)
							{
								tempB=disk_1.search(val);
								for(int l=0;l<tempB->filled_key_amount;l++)
								{	
										if(tempB->keys_array[l]==val)
										{
											text+=tempB->data[l];
											break;			
										}
								}
								val++;
							}
														
							//update metadata vector
							id_val++;
							struct metaNode temp;
							temp.ID=id_val;
							temp.modified_bit=0;
							temp.name=tempName;
							int i=0,c_sum=0;
							while(text[i]) //calculate check sum
							{	
								c_sum+=text[i];
								i++;
							}	
							temp.checksum=c_sum;
							double num_of_blocks=ceil(i/8.0); //to get the number of blocks needed
							temp.numOfBlocks=num_of_blocks;
							temp.size=num_of_blocks*8; 
							metaList.push_back(temp); 	
							
							//store the file in both B trees (RAID level 1)
							
							int remaining_characters=i%8; //get the charaters of the last block
							string subStr;
							int index=0;
							//to split the text string to b byte sized substrings
							for(int i=1;i<num_of_blocks;i++) 
							{		
								subStr = text.substr(index,8); 
								disk_1.insert(id_val,subStr); //insert the data to disk 1 
								disk_2.insert(id_val,subStr); //insert the data to disk 2 
								index+=8;
								id_val++;
							}
							subStr=text.substr(index,remaining_characters); 
							disk_1.insert(id_val,subStr); //insert the last block of data to disk 1 
							disk_2.insert(id_val,subStr); //insert the last block of data to disk 2 	

				}

				flag=1;
				break;
			}	
		}
		
		if(!flag)
		{
			cout<<"File name is invalid, please try again."<<endl;
		}
		else
		{
			break;
		}
	}	
	
}

//to view the content of a file
void viewFile() 
{
	/*	
algorithm
	 1.select the corresponding file
	 2.get both file strings and calculate checksums and store those two values in temp varibles
	 3.IF (file is a modified one) then
		{   get two original checksums from the CoW lists (actually getting one is enough both checksums are same, stored at same time.)
			calculate two saved CoW string's checksums 
			if(original checksum 1 and calculated disk 1's checksum doesn't match)
				if(original checksum 1 and calculated disk 2's checksum doesn't match) 	//both files are corrupted
					error (both files are corrupted)			
				else 																	//only file 1 corrupted
					//UPDATE DISK 1 string
					view disk2 file
			else
				if(original checksum 1 and calculated disk 2's checksum doesn't match) 	//only file 2 corrupted
					//UPDATE DISK 2 string
					view disk 1 file			
				else 																	//both files are okay
					view disk 1,2 file //it can be either 1 or 2
		}
		ELSE
		{
			get the original checksum from the metadata list
			if(original checksum and calculated disk 1's checksum doesn't match)
				if(original checksum and calculated disk 2's checksum doesn't match) 	//both files are corrupted
					error (both files are corrupted)			
				else 																	//only file 1 corrupted
					//UPDATE DISK 1 string
					view disk2 file
			else
				if(original checksum and calculated disk 2's checksum doesn't match) 	//only file 2 corrupted
					//UPDATE DISK 2 string
					view disk 1 file			
				else 																	//both files are okay
					view disk 1,2 file //it can be either 1 or 2
	    }
	
	*/	
	
	//get the input
	string fName;
	int flag=0;
	while(1) //to get the file name from user
	{
		cout<<"Enter the name of the file you want to view: ";
		cin.ignore();
		getline(cin, fName);

		for(int i=0;i<metaList.size();i++) //find the corresponding file
		{
			if(!((metaList[i].name).compare(fName))) //file is there.!
			{
	
				//get the file's content from the trees and calculate both check_sums
							
				string text1,text2; //we recollect the files to these strings from tree blocks
				BTreeNode *tempB1,*tempB2;
				//disk 1
				int val=metaList[i].ID; //val is key
				for(int h=0;h<metaList[i].numOfBlocks;h++)
				{
					tempB1=disk_1.search(val);
					for(int l=0;l<tempB1->filled_key_amount;l++)
					{	
							if(tempB1->keys_array[l]==val)
							{
								text1+=tempB1->data[l];
								break;			
							}
					}
				val++;
				}
				//disk 2
				val=metaList[i].ID; //val is key
				for(int h=0;h<metaList[i].numOfBlocks;h++)
				{	
					tempB2=disk_2.search(val);
					for(int l=0;l<tempB2->filled_key_amount;l++)
					{	
							if(tempB2->keys_array[l]==val)
							{
								text2+=tempB2->data[l];
								break;			
							}
					}
				val++;
				}
											
				int a=0,c_sum1=0,c_sum2=0;
				while(text1[a]) //calculate check sum of disk 1 text
				{	
					c_sum1+=text1[a];
					a++;
				}
							
				a=0;
				while(text2[a]) //calculate check sum of disk 2 text
				{	
					c_sum2+=text2[a];
					a++;
				}

				if(metaList[i].modified_bit) //If the file is already modified
				{
					int	original_c_sum;
					int q=0,f=0,c_sum1=0,c_sum2=0,index_in_CoW=0;
					string t1,t2;
					//get the files vector index
					for (int j=0;j<CoW_List_Disk1.size();j++)  //iterate through the CoW vector
					{
						if(CoW_List_Disk1[j].ID==metaList[i].ID) //find the corresponding file's CoW index
						{
								
								original_c_sum=CoW_List_Disk1[j].checksum; //original checksum (getting only one original is enough)	 
								t1=CoW_List_Disk1[j].modifiedFile; //text from disk 1 CoW list
								t2=CoW_List_Disk2[j].modifiedFile; //text from disk 2 CoW list								
								while(t1[q]) //calculate checksum
								{	
									c_sum1+=t1[q];
									q++;
								}
								while(t2[f]) //calculate checksum  
								{	
									c_sum2+=t2[f];
									f++;
								}
								index_in_CoW=j;
								break;
					
						}					
					}
					//checking equalities of checksums					
					if(original_c_sum!=c_sum1)
					{
						if(original_c_sum!=c_sum2)
						{
							cout<<"Error.! files stored on both disks corrupted"<<endl;
						}
						else
						{
							//update DISK 1's string
							CoW_List_Disk1[index_in_CoW].modifiedFile=CoW_List_Disk2[index_in_CoW].modifiedFile;
							cout<<"Text File Content: "<<t2;
						}
					}
					else
					{
						if(metaList[i].checksum!=c_sum2)
						{
							//UPDATE DISK 2 string
							CoW_List_Disk2[index_in_CoW].modifiedFile=CoW_List_Disk1[index_in_CoW].modifiedFile;
							cout<<"Text File Content: "<<t1;
						}
						else
						{
							cout<<"Text File Content: "<<t1;
						}
					}						
							
				}
			
				else //if the file is not a modified one
				{
					if(metaList[i].checksum!=c_sum1)
					{
						if(metaList[i].checksum!=c_sum2)
						{
							cout<<"Error.! files stored on both disks corrupted"<<endl;
						}
						else
						{
							//update DISK 1's string
							BTreeNode *tempB2,*tempB1;
							//disk 2
							val=metaList[i].ID; //val is key
							for(int h=0;h<metaList[i].numOfBlocks;h++)
							{	
								tempB1=disk_1.search(val);
								tempB2=disk_2.search(val);
								for(int l=0;l<tempB2->filled_key_amount;l++)
								{	
										if(tempB2->keys_array[l]==val)
										{
											tempB1->data[l]=tempB2->data[l]; //updating the disk 1
											break;			
										}
								}
								val++;
							}
							cout<<"Text File Content: "<<text2;
						}
					}
					else
					{
						if(metaList[i].checksum!=c_sum2)
						{
							//UPDATE DISK 2 string
							BTreeNode *tempB2,*tempB1;
							//disk 1
							val=metaList[i].ID; //val is key
							for(int h=0;h<metaList[i].numOfBlocks;h++)
							{	
								tempB1=disk_1.search(val);
								tempB2=disk_2.search(val);
								for(int l=0;l<tempB2->filled_key_amount;l++)
								{	
										if(tempB2->keys_array[l]==val)
										{
											tempB2->data[l]=tempB1->data[l]; //updating the disk 2
											break;			
										}
								}
								val++;
							}
							cout<<"Text File Content: "<<text1;
						}
						else
						{
							cout<<"Text File Content: "<<text1;
						}
					}					
				}
				
				flag=1;
				break;
			}	
		}
		
		if(!flag)
		{
			cout<<"File name is invalid, please try again. "<<endl;
		}
		else
		{
			break;
		}
	}			
}

int main() 
{ 
/*													Description

	maximum number of keys a node can accommodate is 2*(min_deg) - 1 which is equal to 2*5-1=9 (I used 5 as the minimum degree)
	a key is equal to a block, assume block size is 8 bytes 
	A node contains at most 9 blocks, so the extent size is 9*8bytes + size of other data 
	other data-> l bit,min_deg .. etc */	
	
	//The menu of the file system
	int optionNumber;
	cout<<"~~  B-tree File System (Btrfs) Simulation ~~"<<endl;
	while(1)
	{	
		cout<<"\n\n````````` The Main Menu `````````"<<endl;
		cout<<"---------------------------------"<<endl;
		cout<<"1.Create a Text File \n2.Delete a Text File \n3.Modify a Text File \n4.View the Content of a Text File \n5.Create a Copy of a Text File \n6.List Files \n7.Exit\n";
		cout<<"---------------------------------"<<endl;
		cout<<"\nEnter The Option Number: ";
		scanf("\n");	
		cin>>optionNumber;
		cout<<endl;
		
		if(optionNumber==1)
		{
			createFile();	
		}
		else if(optionNumber==2)
		{
			deleteFile();
		}
		else if(optionNumber==3)
		{
			modifyFile();
		}
		else if(optionNumber==4)
		{
			viewFile();
		}
		else if(optionNumber==5)
		{
			 copyFile();
		}
		else if(optionNumber==6)
		{
			listFiles();
		}
		else if(optionNumber==7)
		{
			break;
		}
		else
		{
			cout<<"Invalid Input.! Please Try Again."<<endl;
		}
	}	
	
	return 0; 
} 

