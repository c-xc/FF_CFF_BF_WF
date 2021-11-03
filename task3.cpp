#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

const int SIZE = 1000;

typedef struct free_node *free_ptr;
typedef struct free_node{
	int address;
	int length;
	int flag;
	free_ptr p;
}free_node;
free_ptr free_root = NULL;
free_ptr current = NULL;

typedef struct used_node *used_ptr;
typedef struct used_node{
	int address;
	int length;
	int pid;
	used_ptr p;
}used_node;
used_ptr used_root = NULL;

vector<free_ptr> fp;
bool cmp1(free_ptr p1, free_ptr p2){         // 从小到大 
	return p1->length <= p2->length;
}

bool cmp2(free_ptr p1, free_ptr p2){         // 从大到小 
	return p1->length >= p2->length;
}

void vt_erase(free_ptr fptr){
	for(vector<free_ptr>::iterator iter = fp.begin() ; iter != fp.end() ; ){
		if ((*iter) == fptr)
            iter = fp.erase(iter);
        else
            iter++;
	}
}

void state(){
	printf("------------free memory table------------\n");
	printf("address\t\tlength\t\t  flag\n");
	free_ptr fptr= free_root;
	while(fptr){
		printf("%5d\t\t%5d\t\t%5d\n",fptr->address,fptr->length,fptr->flag);
		fptr = fptr->p;
	}
	printf("\n");
	
	printf("------------used memory table------------\n");
	printf("address\t\tlength\t\t  pid\n");
	used_ptr uptr = used_root;
	while(uptr){
		printf("%5d\t\t%5d\t\t%5d\n",uptr->address,uptr->length,uptr->pid);
		uptr = uptr->p;
	}
	printf("\n");
}

void allocate_ff(int pid, int size){
	bool success = false;
	free_ptr fptr = free_root;
	while(fptr) {
		if(!fptr->flag && fptr->length == size) {
			fptr->flag = 1;
			success = true;
			break;
		}
		else if(!fptr->flag && fptr->length > size) {
			fptr->flag = 1;
			free_ptr tmp= new free_node();
			tmp->address = fptr->address + size;
			tmp->length = fptr->length - size;
			tmp->flag = 0;
			tmp->p = fptr->p;
			fptr->p = tmp;
			fp.push_back(tmp);
			fptr->length = size;
			success = true;
			break;
		}
		fptr = fptr->p;
	}
	
	if(success) {
		printf("FF allocate successfully!\n");
		if(!used_root) {
			used_root = new(used_node);
			used_root->address = fptr->address;
			used_root->length = fptr->length;
			used_root->pid = pid;
			used_root->p = NULL;
//			used_ptr uptr = used_root;	
		}
		else {
			used_ptr uptr = used_root;
			while(uptr->p) {
				uptr = uptr->p;
			}
			used_ptr uptr2 = new(used_node);
			uptr2->address = fptr->address;
			uptr2->length = fptr->length;
			uptr2->pid = pid;
			uptr2->p = NULL;
			uptr->p = uptr2;
		}
	}
	else printf("FF allocate unsuccessfully!\n"); 
	
}

void allocate_cff(int pid, int size){
	if(!current) current = free_root;
	free_ptr fptr = current;
	while(1) {
		if(!fptr->flag && fptr->length == size) {
			fptr->flag = 1;
			printf("CFF allocate successfully!\n");
			current = fptr->p;
			break;
		}
		else if(!fptr->flag && fptr->length > size) {
			fptr->flag = 1;
			free_ptr tmp= new free_node();
			tmp->address = fptr->address + size;
			tmp->length = fptr->length - size;
			tmp->flag = 0;
			tmp->p = fptr->p;
			fptr->p = tmp;
			fp.push_back(tmp);
			fptr->length = size;
			printf("CFF allocate successfully!\n");
			current = tmp;
			break;
		}
		
		fptr = fptr->p;
		if(fptr == current) {
			printf("CFF allocate unsuccessfully!\n");
			return ;
		}
		if(!fptr) fptr = free_root;
	}
	
	if(!used_root) {
		used_root = new(used_node);
		used_root->address = fptr->address;
		used_root->length = fptr->length;
		used_root->pid = pid;
		used_root->p = NULL;
//		used_ptr uptr = used_root;	
	}
	else {
		used_ptr uptr = used_root;
		while(uptr->p) {
			uptr = uptr->p;
		}
		used_ptr uptr2 = new(used_node);
		uptr2->address = fptr->address;
		uptr2->length = fptr->length;
		uptr2->pid = pid;
		uptr2->p = NULL;
		uptr->p = uptr2;
	}
	
}

void allocate_bf(int pid, int size){
	sort(fp.begin(), fp.end(), cmp1);
	
	bool success = false;
	int i;
	for(i=0 ; i<fp.size() ; i++) {
		if(!fp[i]->flag && fp[i]->length == size) {
			fp[i]->flag = 1;
			success = true;
			break;
		}
		else if(!fp[i]->flag && fp[i]->length > size) {
			fp[i]->flag = 1;
			free_ptr tmp= new free_node();
			tmp->address = fp[i]->address + size;
			tmp->length = fp[i]->length - size;
			tmp->flag = 0;
			tmp->p = fp[i]->p;
			fp[i]->p = tmp;
			fp.push_back(tmp);
			fp[i]->length = size;
			success = true;
			break;
		}
		
	}
	
	if(success) {
		printf("BF allocate successfully!\n");
		if(!used_root) {
			used_root = new(used_node);
			used_root->address = fp[i]->address;
			used_root->length = fp[i]->length;
			used_root->pid = pid;
			used_root->p = NULL;
//			used_ptr uptr = used_root;	
		}
		else {
			used_ptr uptr = used_root;
			while(uptr->p) {
				uptr = uptr->p;
			}
			used_ptr uptr2 = new(used_node);
			uptr2->address = fp[i]->address;
			uptr2->length = fp[i]->length;
			uptr2->pid = pid;
			uptr2->p = NULL;
			uptr->p = uptr2;
		}
	}
	else printf("BF allocate unsuccessfully\n");
	
}

void allocate_wf(int pid, int size){
	sort(fp.begin(), fp.end(), cmp2);
	int i;
	for(i=0 ; i<fp.size() ; i++) {
		if(!fp[i]->flag) {
			if(fp[i]->length == size) {
				fp[i]->flag = 1;
				printf("WF allocate successfully!\n");
				break;
			}
			else if(!fp[i]->flag && fp[i]->length > size) {
				fp[i]->flag = 1;
				free_ptr tmp= new free_node();
				tmp->address = fp[i]->address + size;
				tmp->length = fp[i]->length - size;
				tmp->flag = 0;
				tmp->p = fp[i]->p;
				fp[i]->p = tmp;
				fp.push_back(tmp);
				fp[i]->length = size;
				printf("WF allocate successfully!\n");
				break;
			}
			else {
				printf("WF allocate unsuccessfully!\n");
				return ;
			}
		}
		
	}
	
	if(!used_root) {
		used_root = new(used_node);
		used_root->address = fp[i]->address;
		used_root->length = fp[i]->length;
		used_root->pid = pid;
		used_root->p = NULL;
//		used_ptr uptr = used_root;	
	}
	else {
		used_ptr uptr = used_root;
		while(uptr->p) {
			uptr = uptr->p;
		}
		used_ptr uptr2 = new(used_node);
		uptr2->address = fp[i]->address;
		uptr2->length = fp[i]->length;
		uptr2->pid = pid;
		uptr2->p = NULL;
		uptr->p = uptr2;
	}
}

void reclaim(int pid){
	used_ptr uptr1 = used_root;
	used_ptr uptr2 = NULL;
	int address;
	while(uptr1) {
		if(uptr1->pid == pid) {
			address = uptr1->address;
			if(uptr2 == NULL) used_root = uptr1->p;
			else uptr2->p = uptr1->p;
			free(uptr1);
			break;
		}
		uptr2 = uptr1;
		uptr1 = uptr1->p;
	}
	
	free_ptr fptr1 = free_root;
	free_ptr fptr2 = NULL;
	while(fptr1) {
		if(fptr1->address == address) {
			free_ptr fptr3 = fptr1->p;
			if( fptr2 && !fptr2->flag && (fptr2->address + fptr2->length)==address ) {
				fptr2->length += fptr1->length;
				fptr2->p = fptr1->p;
				free(fptr1);
				vt_erase(fptr1);
			}
			else {
				fptr1->flag = 0;
				fptr2 = fptr1;
			}
			if( fptr3 && !fptr3->flag && (fptr2->address + fptr2->length) == fptr3->address ) {
				fptr2->length += fptr3->length;
				fptr2->p = fptr3->p;
				free(fptr3);
				vt_erase(fptr3);
				break;
			}
//			fptr1->flag = 0;
			break;
		}
		fptr2 = fptr1;
		fptr1 = fptr1->p;
	}
}

void menu(int algo_select){
	printf("menu:\n");
	printf("\t1.allocate\t2.reclaim\t0.exit\n");
	int select;
	printf("select:");
	scanf("%d",&select);
	if(select == 1) {
		
		int pid,size;
		printf("pid:");
		scanf("%d",&pid);
		printf("size:");
		scanf("%d",&size);
		
		if(algo_select == 1)  allocate_ff(pid,size);
		else if(algo_select == 2) allocate_cff(pid,size);
		else if(algo_select == 3) allocate_bf(pid,size);
		else if(algo_select == 4) allocate_wf(pid,size);
		state();
	}
	else if(select == 2) {
		int pid;
		printf("pid:");
		scanf("%d",&pid);
		reclaim(pid);
		state();
	}
	else if(select==0) exit(0);
}


int main()
{
	free_root = new free_node(); 
	free_root->address = 0;
	free_root->length = SIZE;
	free_root->flag = 0;
	free_root->p = NULL;
	fp.push_back(free_root);
	
	state();
	
	printf("\t\t1.FF  2.CFF  3.BF   4.WF\n");
	printf("algorithm select:");
	int algo_select;
	scanf("%d", &algo_select);
		
	while(getchar()) {
		menu(algo_select);
		getchar();
	}
	
	return 0;
}
