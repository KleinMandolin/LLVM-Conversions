// CptS 360 Lab 1
// Samantha Brewer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

typedef struct node {
	char  name[64];       // node's name string
	char  type;
	struct node *child, *sibling, *parent;
} NODE;


// global variables
NODE *root; 
NODE *cwd;
/*
char *cmd[] = {"mkdir","rmdir","ls","cd","pwd","creat",
                "rm","reload","save","quit"};  // fill with list of commands
*/
// ^ command array unnecessary, readability improved if commands actually written out


int initialize() {
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->name, "/");
	root->parent = root;
	root->sibling = 0;
	root->child = 0;
	root->type = 'D';
	cwd = root;
	printf("Filesystem initialized!\n");
}

// creates a new node, automatically inserting it into
// the pointer pipeline using the given parent node
NODE* newNode(char name[64], char type, NODE* parent){
    NODE* madeNode=(NODE*)malloc(sizeof(NODE));
    strcpy(madeNode->name,name);
    madeNode->type=type;
    madeNode->child=0;
    madeNode->parent=parent;
    // pointers have parent's child go to youngest child
    // and sibling pointers go in the direction of the oldest sibling
    // so when new node is added, its sibling is what was the youngest child
    // (easier than parent->child being oldest child and sibling to youngest sibling)
    if(parent->child==0){ // if no children already exist
        madeNode->sibling=0; // newNode has no siblings
    } else {
        madeNode->sibling=parent->child; // newNode points towards old youngest child of parent
    }
    parent->child=madeNode;
    return madeNode;
}

int removeNode(NODE* delete){
    NODE* currentNode=delete->parent->child;
    if(delete==0){
        printf("ERROR: Node doesn't exist!\n");
        return 0;
    }

    if(delete==root){
        printf("ERROR: Root cannot be deleted!\n");
        return 0;
    }

    // if youngest child is delete, change youngest to delete's sibling (second youngest)
    if(delete->parent->child==delete){
        delete->parent->child=delete->sibling;

    // if not youngest child, find older sibling
    } else {
        while(currentNode->sibling!=delete){
            currentNode=currentNode->sibling;
        }
        currentNode->sibling=delete->sibling; // a->b->c becomes a->c
    }
    free(delete);
    return 1;
}

// finds a specific node in given parent's children
// specifiying type in case of duplicate names
NODE* findNodeinChildren(char nodeName[64], NODE* parent, char type){
    NODE* currentNode;
    int duplicateWrongTypeFound=0;
    if(parent->child==0){
        return 0;
    }
    currentNode=parent->child;
    if(type=='\0'){ // if type doesn't matter
        while(strcmp(currentNode->name,nodeName)!=0){
            if(currentNode->sibling==0){
                return 0; // end of siblings and no match
            }
            currentNode=currentNode->sibling;
        }
    } else { // to make sure it returns correct node if there are duplicates e.g.
                // "test" file and "test" directory, don't just return first encounter of name
        while(strcmp(currentNode->name,nodeName)!=0 && currentNode->type!=type){
            if (strcmp(currentNode->name,nodeName)==0){
                duplicateWrongTypeFound=1;
            }
            if(currentNode->sibling==0){
                if(duplicateWrongTypeFound==1){ // only match is for wrong file type
                    return parent;
                }
                return 0; // end of siblings and no match
            }
            currentNode=currentNode->sibling;
        }
    }
    if(currentNode->type!=type){
        return parent;
    } else if (strcmp(currentNode->name,nodeName)!=0){
        return 0;
    } else {
        return currentNode;
    }
}


// returns node if found, 0 if not, parent if only wrong type
// finds the node given a directory, returning 0 if not found
// assumes directory is not empty
// SOLELY used for directory names, assumes
// that it is a directory node it will return
// and is used with findNodeinChildren
NODE* findDirNode(char pathname[200]){
    NODE* temp;
    char origDirectory[200];
    strcpy(origDirectory,pathname);
    NODE* currentNode;
    char* currentName;

    if (origDirectory[0]=='/'){ // start from absolute pathname
        currentNode=root;
    } else { // start from working directory
        currentNode=cwd;
    }

    currentName=strtok(origDirectory,"/\0");

    while(currentName){
        // find node with current section name
        // recursively runs so that previous matching child Directory
        // is now parent Node in next find matching child call
        temp=findNodeinChildren(currentName,currentNode,'D');
        if(temp==0 || temp==currentNode) {
            return 0; // no match
        }

        // once match on current rank found
        // go to next part of directory name
        // and next level of nodes
        currentName=strtok(NULL,"/\0");
        currentNode=temp;
    }
    // end of directory or error not encountered, thus match is found
    // return node of end of pathname
    return currentNode;
}

// recursively prints absolute (starts at root) pathname of
// current node (recursive from lowest node as child->parent is quicker)
// solely called in main with cwd, current is for recursive calls
void pwd(NODE* current){
    // example of recursive calls
    // result:   /child/grandchild
    // where pwd(grandchild) calls pwd(child) calls pwd(root)
    // result: "grandchild" after "child/" after "/"

    if(current==root){ // if current node is root
        printf("/");
        return;
    } else {
        pwd(current->parent);
        if(current->child==0 || current==cwd){ // if end of directory OR start of pwd
            printf("%s",current->name);
        } else {
            printf("%s/",current->name);
        }
    }
}

// same as pwd but to file stream and given node
void printPath(FILE* stream, NODE* current){
    if(current==root){ // if current node is root
        fprintf(stream,"/");
        return;
    } else {
        printPath(stream,current->parent);
        if(current->child==0){ // if end of directory
            fprintf(stream,"%s",current->name);
        } else {
            fprintf(stream,"%s/",current->name);
        }
    }
}

// recursive function, saving current node's
// type and filepath to given stream file
void save(FILE* stream, NODE* current){
    if(current!=0){
        fprintf(stream,"%c ",current->type);
        printPath(stream,current);
        fprintf(stream,"\n");
    }
    if(current->child!=0){
        save(stream,current->child);
    }
    if(current->sibling!=0){
        save(stream,current->sibling);
    }
}

// recursive function to resetTree without
// any hanging pointers
void resetTree(NODE* current){
    if(current->child!=0){
        resetTree(current->child);
    }
    if(current->sibling!=0){
        resetTree(current->sibling);
    }
    free(current);
}

// resets filesystem tree and parses given stream file
// and creates each given entry in new tree
void reload(FILE* stream){
    resetTree(root);
    initialize();
    char* type;
    char* path;
    char currentLine[200];
    char* reverse;
    char* basename;
    char* dirname;

    char typeC;

    fgets(currentLine,200,stream);

    while(fgets(currentLine,200,stream)!=NULL){
        type=strtok(currentLine, " ");
        path=strtok(NULL,"\n");

        if(strcmp(type,"D")==0){
            typeC='D';
        } else {
            typeC='F';
        }

        reverse=strrev(path);
        basename=strtok(reverse,"/");
        dirname=strtok(NULL,"\0 ");
        basename=strrev(basename);
        if(dirname== NULL){
            newNode(basename,typeC,root);
        } else {
            dirname=strrev(dirname);
            NODE* temp1= findDirNode (dirname);
            newNode(basename,typeC,temp1);
        }
    }

}

int main() {
	initialize();
	// other initialization as needed

    NODE* temp1; // node pointer used for placeholders
    NODE* temp2;

    // used for parsing pathnames
    char *dirname;
    char *basename;
    char *reverse;

    char entireCommand[200]="NULL"; // assuming command strings won't have long directories
    char *commandName; // for commands that have extra information e.g. mkdir (name)
    char *additionalInfo; // the addon information e.g. name of new directory in mkdir
	while(strcmp(entireCommand,"quit")!=0) {
		printf("Enter command: ");
		fgets(entireCommand, 200, stdin);
        // strtok functions include \n in delimiter to avoid it tampering
        // with strcmp functions
        commandName=strtok(entireCommand, " \n");
        additionalInfo=strtok(NULL,"\n");


        /*
        // split directory into dirname and basename
        reverse=strrev(additionalInfo);
        basename=strtok(reverse,"/");
        dirname=strtok(NULL,"\0 ");
        basename=strrev(basename);
        dirname=strrev(dirname);

        printf("\ncommandName:%s\n", commandName);
        printf("additionalInfo:%s\n", additionalInfo); // =basename, unusable
        printf("basename:%s\n",basename);
        printf("dirname:%s\n",dirname);
        printf("OrigPath:%s/%s\n",dirname,basename);
        */


        // find command used

        // mkdir = make directory
        if(strcmp(commandName,"mkdir")==0){
            if(!(additionalInfo)){
                printf("ERROR: No pathname specified!\n");
            } else {
                // split into dirname and then name of new directory
                // if given directory (e.g. /a/b versus just mkdir b)
                if(strchr(additionalInfo, '/') != NULL){
                    reverse=strrev(additionalInfo);
                    basename=strtok(reverse,"/");
                    dirname=strtok(NULL,"\0 ");
                    basename=strrev(basename);
                    dirname=strrev(dirname);

                    temp1= findDirNode (dirname);
                    if(temp1==0) {
                        printf ("ERROR: DIR %s does not exist!\n", dirname);
                    } else {
                        temp2= findNodeinChildren (basename,temp1,'D');
                        if(temp1==temp2 || temp2==0){ // if only match is diff type or no match
                            newNode(basename,'D',temp1);
                        } else {
                            printf("ERROR: DIR %s/%s already exists!\n",dirname,basename);
                        }
                    }
                // if no pathname e.g. mkdir hello
                } else {
                    temp1=findNodeinChildren(additionalInfo,cwd,'D');
                    if(temp1==cwd || temp1==0){
                        temp2=newNode(additionalInfo,'D',cwd);
                    } else {
                        printf("ERROR: Dir %s already exists!\n",additionalInfo);
                    }
                }

            }

        // rmdir = remove directory
        } else if (strcmp(commandName,"rmdir")==0){
            if(!(additionalInfo)){
                printf("ERROR: No pathname specified!\n");
            } else {
                // if complex filepath (contains / so is multiple ranks)
                if(strchr(additionalInfo, '/') != NULL) {
                    temp1= findDirNode (additionalInfo);
                    if(temp1==0) {
                        printf ("ERROR: DIR %s does not exist!\n", additionalInfo);
                    } else {
                        if(temp1->child==NULL){
                            removeNode (temp1);
                        } else {
                            printf("ERROR: DIR %s is not empty!\n",additionalInfo);
                        }
                    }
                // if simple directory e.g. rmdir hello
                } else {
                    temp1=findNodeinChildren(additionalInfo,cwd,'D');
                    if(temp1==cwd || temp1==0){
                        printf ("ERROR: DIR %s does not exist!\n", additionalInfo);
                    } else {
                        if(temp1->child==NULL){
                            removeNode (temp1);
                        } else {
                            printf("ERROR: DIR %s is not empty!\n",additionalInfo);
                        }
                    }
                }
            }


        // cd = change working directory
        // NOTE: assumed "cd ", "cd ..", and "cd pathname" are only options
        //       as assignment does not specify
        } else if (strcmp(commandName,"cd")==0){
            if (!(additionalInfo)){
                cwd=root;
            } else if(strcmp(additionalInfo,"..")==0){
                cwd=cwd->parent;
            } else {
                temp1= findDirNode (additionalInfo);
                if(temp1==0){
                    printf("ERROR: Directory does not exist!\n");
                } else {
                    cwd=temp1;
                }
            }

        // ls = list children nodes of given directory
        } else if (strcmp(commandName,"ls")==0){
            if(!(additionalInfo)){
                temp1=cwd;
            } else {
                temp1=findDirNode (additionalInfo);
            }

            if(temp1==0){
                printf("ERROR: Directory does not exist!\n");
            } else {
                if(temp1->child!=0){
                    temp2=temp1->child;
                    while(temp2!=0){
                        printf("%c %s\n",temp2->type,temp2->name);
                        temp2=temp2->sibling;
                    }
                }
            }


        // pwd = print working directory
        } else if (strcmp(commandName,"pwd")==0){
            if(additionalInfo){
                printf("ERROR: pwd should not have any other information!\n");
            } else {
                pwd(cwd);
                printf("\n");
            }

        // creat = create file node
        } else if (strcmp(commandName,"creat")==0){
            if(!(additionalInfo)){
                printf("ERROR: No pathname specified!\n");
            } else {
                // split into dirname and then name of new file
                // if given directory (e.g. /a/b versus just mkdir b)
                if(strchr(additionalInfo, '/') != NULL){
                    reverse=strrev(additionalInfo);
                    basename=strtok(reverse,"/");
                    dirname=strtok(NULL,"\0 ");
                    basename=strrev(basename);
                    dirname=strrev(dirname);

                    temp1= findDirNode (dirname);
                    if(temp1==0) {
                        printf ("ERROR: DIR %s does not exist!\n", dirname);
                    } else {
                        temp2= findNodeinChildren (basename,temp1,'F');
                        if(temp1==temp2 || temp2==0){ // if only match is diff type or no match
                            newNode(basename,'F',temp1);
                        } else {
                            printf("ERROR: File %s/%s already exists!\n",dirname,basename);
                        }
                    }
                } else {
                    temp1=findNodeinChildren(additionalInfo,cwd,'F');
                    if(temp1==cwd || temp1==0){
                        temp2=newNode(additionalInfo,'F',cwd);
                    } else {
                        printf("ERROR: File %s already exists!\n",additionalInfo);
                    }
                }

            }

        // rm = remove file
        } else if (strcmp(commandName,"rm")==0){
            if(!(additionalInfo)){
                printf("ERROR: No pathname specified!\n");
            } else {
                // split into dirname and then name of new file
                // if given directory (e.g. /a/b versus just mkdir b)
                if(strchr(additionalInfo, '/') != NULL){
                    reverse=strrev(additionalInfo);
                    basename=strtok(reverse,"/");
                    dirname=strtok(NULL,"\0 ");
                    basename=strrev(basename);
                    dirname=strrev(dirname);

                    temp1= findDirNode (dirname);
                    if(temp1==0) {
                        printf ("ERROR: DIR %s does not exist!\n", dirname);
                    } else {
                        temp2= findNodeinChildren (basename,temp1,'F');
                        if(temp2==0) {
                            printf ("ERROR: File %s/%s doesn't exist!\n", dirname, basename);
                        } else if (temp1==temp2){
                            printf("ERROR: %s is not a file!\n",basename);
                        } else {
                            removeNode (temp2);
                        }
                    }
                } else {
                    temp2=findNodeinChildren(additionalInfo,cwd,'F');
                    if(temp2==0) {
                        printf ("ERROR: File %s doesn't exist!\n", additionalInfo);
                    } else if (temp2==cwd){
                        printf("ERROR: %s is not a file!\n",additionalInfo);
                    } else {
                        removeNode (temp2);
                    }
                }
            }

        // save = save filetree to file fssim_brewer.txt
        } else if (strcmp(commandName,"save")==0){
            if(additionalInfo) {
                printf ("ERROR: save should not have any other information!\n");
            } else {
                FILE *fp = fopen ("fssim_brewer.txt", "w+");
                save (fp, root);
                fclose (fp);
            }

        // reload = resets tree and loads tree from gile fssim_brewer.txt
        } else if (strcmp(commandName,"reload")==0){
            if(additionalInfo) {
                printf ("ERROR: reload should not have any other information!\n");
            } else {
                FILE *fp=fopen("fssim_brewer.txt","r");
                reload(fp);
                fclose(fp);
            }

        // quit = saves current tree layout to fssim_brewer.txt and closes
        } else if (strcmp(commandName,"quit")==0){
            if(additionalInfo) {
                printf ("ERROR: quit should not have any other information!\n");
            } else {
                printf("Exiting...\n");
                FILE *fp=fopen("fssim_brewer.txt", "w+");
                save(fp,root);
                fclose(fp);
            }
        } else {
            printf("ERROR: Command not found!\n");
        }


        /*
        printf("\ncommandName: %s\n", commandName);
        printf("additionalInfo: %s\n", additionalInfo);
        if(!(additionalInfo)){
            printf("if statement works");
        }
        */

	}


    return 0;
}