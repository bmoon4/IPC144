/*
Name	: Bokyung Moon
Student number: 136103165
Email : bmoon4@myseneca.ca
Date	: 10-April-2017
Milestone 4 Final Assembly- short 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>   //Header Files


#define LINEAR 1
#define FORM 0
#define STOCK 1
#define CHECKOUT 0
#define MAX_QTY 999
#define SKU_MAX 999
#define SKU_MIN 100
#define MAX_ITEM_NO 21
#define DATAFILE "items.txt"  //Symbolic Constants

//declaring a structure
struct Item {
  double price;
  int sku;
  int isTaxed;
  int quantity;
  int minQuantity;
  char name[21];
};
const double TAX = 0.13; //Global Constant for Tax


// ms1 prototypes
void welcome(void);
void GroceryInventorySystem(void);
void printTitle(void);
void printFooter(double gTotal);
void pause(void);
void flushKeyboard(void);
int getInt(void);
double getDouble(void);
int getIntLimited(int lowerLimit, int upperLimit);
double getDoubleLimited(double lowerLimit, double upperLimit);
int yes(void);  
void MenuChoice(void);


//ms2 prototypes
double totalAfterTax(struct Item item);
int isLowQuantity(struct Item item);
struct Item itemEntry(int sku);
void displayItem(struct Item item, int linear);
void listItems(const struct Item item[], int NoOfItems);
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index);

// ms3 prototypes
void search(const struct Item item[], int NoOfRecs);
void updateItem(struct Item* itemptr);
void addItem(struct Item item[], int *NoOfRecs, int sku);
void addOrUpdateItem(struct Item item[], int* NoOfRecs);
void adjustQuantity(struct Item item[], int NoOfRecs, int stock);

 
// ms4 prototypes
void saveItem(struct Item item, FILE* dataFile);
int loadItem(struct Item* item, FILE* dataFile);
int saveItems(const struct Item item[], char fileName[], int NoOfRecs);
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr);




//---------main----------------------
int main(void){
	GroceryInventorySystem();
	return 0;
}
//----------------------------------


//ms1 functions here
void welcome(void){ // print welcome head
	printf("---=== Grocery Inventory System ===---\n");
}
void GroceryInventorySystem(void){
	struct Item item[MAX_ITEM_NO];
	int option;
	int index;
	int yesInput = '\0';
	int call;
	int records=20;
    int result;
 
	welcome();
	printf("\n");
	call = loadItems(item, DATAFILE, &records); // Calling loaditems() to load data from the file("items.txt") to the item array
	
	if(call != 1)
    	printf("Could not read from %s.\n",DATAFILE);
    else{
            do{
   	   		MenuChoice(); // showing the menu
       		scanf("%d", &option);

      		switch (option){
     		 	case 0: 
       			printf("Exit the program? (Y)es/(N)o: ");  //exit the program
			    yesInput = yes();
				break;
			    
				case 1: 
         		listItems(item,records); // show list of items from the structure
         		rewind(stdin);  // empty the buffer
         	    pause();
      			break;
			
				case 2: 
   	   			search(item,records); // search items from the structure
   	   			rewind(stdin); //empty the buffer
         	    pause();
   	   			break;
      		
			  	case 3: 
	  		    adjustQuantity(item, records, CHECKOUT); //Call the adjustQuantity function passing the item array, the number of records and CHECKOUT.
	  		    rewind(stdin); // empty the buffer
         	    pause();
	  		    result =saveItems(item, DATAFILE, records); 
	  		     	if(result!= 1){
                     	printf("Could not update data file!\n"); //if fails to save data, prints "could not update data file!".
	  		    	 }
   	   			break;
      		
			  	case 4: 
	  			adjustQuantity(item, records, STOCK); //Call the adjustQuantity function passing the item array, the number of records and STOCK.
	  			rewind(stdin); // empty the buffer
         	    pause();
	  		    result =saveItems(item, DATAFILE, records);
	  		     	if(result!= 1){
                     	printf("Could not update data file!\n");//if fails to save data, prints "could not update data file!".
	  		     	}
				break;
	  		
			  	case 5: 
				addOrUpdateItem(item, &records); //Call the addOrUpdateItem function passing the item array and the of the number of records.
				rewind(stdin);// empty the buffer
         	    pause();
				result =saveItems(item, DATAFILE, records);
					if(result!= 1){
                     	printf("Could not update data file!\n"); //if fails to save data, prints "could not update data file!"
	  		     	}
				break;
	  		
			  	case 6: 
	   			printf("Not implemented!\n");
         	    pause();
	  			break;
	  			
				case 7: 
	  			printf("Not implemented!\n");
         	    pause();
	  			break;
      		
			  	default:
        	 	printf("Error: Please enter a valid option to continue\n");
         		break;
      }
   }while(yesInput!=1); 
   }
}
void printTitle(void){ // print title
	printf("Row |SKU| Name               | Price  |Taxed | Qty | Min |   Total    |Atn\n");
	printf("----+---+--------------------+--------+------+-----+-----+------------|---\n");
}
void printFooter(double gTotal){ //print footer
	if(gTotal>0){
	printf("---------------------------------------------------------+----------------\n");
	printf("                                           Grand Total:  |     %-12.2lf\n", gTotal);
		}		
    else{
   	printf("---------------------------------------------------------+----------------\n");
   }	
}
void flushKeyboard(void) {
	while (getchar() != '\n');
}
void pause(void) {
	printf("Press <ENTER> to continue...");
	while (getchar() != '\n');
}
int getInt(void){   // get Integer  
	int VALUE;
	char NL= 'x';
    	scanf("%d%c", &VALUE, &NL);
	while(NL !='\n'){  // if the last input is not 'Enter(\n)' but other character
	 rewind(stdin);
	 printf("Invalid integer, please try again: ");
	 scanf ("%d%c", &VALUE, &NL);	
	}
	return VALUE;
	}
double getDouble(void){ //get double
	double VALUE;
	char NL= 'x';
    scanf("%lf%c", &VALUE, &NL);
	while(NL !='\n'){
	 rewind(stdin);
	 printf("Invalid number, please try again: ");
	 scanf ("%lf%c", &VALUE, &NL);	
	}
	return VALUE;
	}
int getIntLimited(int lowerLimit, int upperLimit){
	int number; ////getIntLimited checking the limits of Input
	scanf("%d", &number);
    while (number < lowerLimit || number > upperLimit){
    	printf("Invalid value, %d < value < %d: ", lowerLimit, upperLimit);
    	scanf("%d", &number);  	
	}
	return number;	
}
double getDoubleLimited(double lowerLimit, double upperLimit){
	double number;   ////also getDoubleLimited checking the limits of Input as well
	scanf("%lf", &number);
    while (number < lowerLimit || number > upperLimit){
    	printf("Invalid value, %lf < value < %lf : ", lowerLimit, upperLimit);
    	scanf("%lf", &number); 	
	}
	return number;
}
int yes(void){ // checking whether the answer is 'yes' or not
    char input;
	begin:;
	rewind(stdin);
    scanf("%c", &input);    
    if (input == 'Y' || input == 'y'){ // if the input is 'Y' or 'y'
         return 1;                     // returns 1
    } else if(input == 'N'|| input =='n'){
         return 2;
	} else{
		 printf("Only (Y)es or (N)o are acceptable: "); // if the input is neither Y nor N
		 goto begin;                 // go back to the begin and start it over again
	}                                 // until receive Y or N (y, n)
}
void MenuChoice(void){  // showing the menu
         printf("1- List all items\n"
                "2- Search by SKU\n"
                "3- Checkout an item\n"
                "4- Stock an item\n"
                "5- Add new item or update item\n"
                "6- Delete item\n"
                "7- Search by name\n"
                "0- Exit program\n"
                "> ");
}
	
	
//------------------------------------
// ms2 functions here
double totalAfterTax(struct Item item){ // Calculating Tax
	double total=0;
	if (item.isTaxed == 0){  // if an item is not needed to be tax calculated
		total = item.price*item.quantity;  // simply multiplies without tax rate (0.13)
		return total;
	} 
	else{
		total = item.price * item.quantity;  // // if an item is needed to be tax calculated
		total = total + (total * TAX);  // use tax rate
		return total;
    }	
}
int isLowQuantity(struct Item item){  // Checking if the quantity is lower than minimum quantity
	if(item.quantity < item.minQuantity){
		return 1;
	}
	else{
		return 0;
	}	
}
struct Item itemEntry(int sku){   // item entry
	struct Item itemEntry;
    char tax;
    int price;
    int qty;
    int min_qty;
    
	itemEntry.sku = sku;
	printf("        SKU: %d\n", sku);
	printf("       Name: ");
	scanf("%20[^\n]", itemEntry.name);
	flushKeyboard();
	printf("      Price: ");
	price = getDouble();
	itemEntry.price = price;
	printf("   Quantity: ");
	qty = getInt();
	itemEntry.quantity= qty;
	printf("Minimum Qty: ");
	min_qty = getInt();
	itemEntry.minQuantity= min_qty;
	printf("   Is Taxed: ");
	tax = yes();
    itemEntry.isTaxed = tax; 
	
	return itemEntry;
}
void displayItem(struct Item item, int linear){ // display items
	char* yesno[4];  // this array is for converting digit into characters
	double total;    
	
	if(item.isTaxed == 1){ // if the item is taxed
		*yesno = "Yes";    // array contains "Yes"
		total = item.price*item.quantity*(1 + TAX);}
		else {             // if the item is not taxed
			*yesno = "No"; // array contains "No"
			total = item.price*item.quantity;
			
		}
		
		if(linear == 1){   // linear format
		printf("|%3d|%-20s|%8.2lf| %3s|  %3d|   %3d|%12.2lf|***\n", item.sku, 
		item.name, item.price, *yesno, item.quantity, item.minQuantity, total);
		
			if(item.quantity < item.minQuantity){  // if the quantity is lower than minimum
			 	printf("***\n");        // put "***" as an alarm
			}	
			else {
				printf("\n");
			}
	    }else{  //Form format
			printf("        SKU: %d\n", item.sku);
			printf("       Name: %s \n", item.name);
			printf("      Price: %.2lf\n", item.price);
			printf("   Quantity: %d\n", item.quantity);
			printf("Minimum Qty: %d\n", item.minQuantity);
		    printf("   Is Taxed: %s\n", *yesno);
			
			if(item.quantity < item.minQuantity){ // if the quantity is lower than minimum
			printf("WARNING: Quantity low, please order ASAP!!!\n"); // print warning
			}
	}
}
void listItems(const struct Item item[], int NoOfItems){  // list items
	int i;
	double total =0;
	double gtotal= 0;
	int Low_or_not;
	
	printTitle();
	
	for(i=0; i < NoOfItems ; i++){
		char* yesno[4];
		if(item[i].isTaxed == 1){
			*yesno = "yes";
			total = item[i].price*item[i].quantity*(1 + TAX);
		}
		else{
			*yesno = "No";
			total = item[i].price*item[i].quantity;
		}
		gtotal += total;  // calulate grand total
		
		printf("%-4d|%3d|%-20s|%8.2lf|   %3s| %3d | %3d |%12.2lf|", i+1, item[i].sku, item[i].name, 
		item[i].price, *yesno, item[i].quantity, item[i].minQuantity, total);
		
		Low_or_not =isLowQuantity(item[i]); //// if the quantity is lower than minimum
		if(Low_or_not==1){
			printf("***\n");	// put "***" as an alarm
		}
		else{
			printf("\n");
		}
	}
	printFooter(gtotal);
	
}
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index){ // find the location of item
	int i;
	int flag = 0;   // flag will be used to indicate index of array
	
	for (i = 0; i <NoOfRecs ; i++){
		if(sku == item[i].sku){
			*index = i;
			flag =1;		
		}
	}
	return flag;  //returns location of item
}

//ms3 functions here

void search(const struct Item item[], int NoOfRecs){ // search a specific item by using sku number
	int sku;
	int i;
	int found;
	
	printf("Please enter the SKU: ");
	sku = getIntLimited(SKU_MIN, SKU_MAX); 
	found = locateItem(item, NoOfRecs, sku, &i);
	if(found==1){
		displayItem(item[i], FORM);  // if find the matched item, display.
	}
	else{ 
	printf("Item not found\n");
	}	
}
void updateItem(struct Item* itemptr){ // update item
	struct Item item;
	int response;
	
	flushKeyboard();
	printf("Enter new data: \n");
	item = itemEntry(itemptr -> sku); 
	printf("overwrite old data? (Y)es/(N)o: ");  
	response = yes();
	
	if(response == 1){
		*itemptr=item;
		printf("__== Updated! ==__\n");
	}
	else{
		printf("__== Aborted! ==__\n");
	}
}
void addItem(struct Item item[], int *NoOfRecs, int sku){ // add item
	struct Item box;
	int Array;
	int AddItem;
	flushKeyboard();
	Array = (*NoOfRecs < MAX_ITEM_NO) ? 1 : 0; // using ternary operator
	// if the condition is true, Array will be '1', otherwise '2'.
	                                            
	if (Array == 1){
		box = itemEntry(sku);
		printf("Add Item? (Y)es/(N)o: ");
		AddItem = yes();

		if (AddItem == 1){
			*NoOfRecs += 1;
			item[*NoOfRecs-1] = box;
			printf("--== Added! ==--\n");
		} 
		else{
			printf("--== Aborted! ==--\n");
		}
	}
	else
	{
		printf("Can not add new item; Storage Full!\n");
	}
}
void addOrUpdateItem(struct Item item[], int* NoOfRecs){ // add or update items
	int sku;
	int i;
	int found;
	int Update;
	
	printf("Please enter the SKU: ");
	sku = getIntLimited(SKU_MIN, SKU_MAX); // receive SKU within the range
	found = locateItem(item, *NoOfRecs, sku, &i);  // find the location of the item which SKU is indicating
	if (found == 1){  // find	
		displayItem(item[i], FORM);
		printf("Item already exists, Update? (Y)es/(N)o: ");
		Update = yes();
        
		if (Update == 1) { // update
			updateItem(&item[i]);
		}
		else{
			printf("--== Aborted! ==--\n");
		}
	}
	else  // add
		addItem(item, NoOfRecs, sku);
}
void adjustQuantity(struct Item item[], int NoOfRecs, int stock){ // adjust quantity of items
	 int sku, flag;
	 int i=-1, k;
	 int Low_or_not;
	 
     rewind(stdin);
     printf("Please enter the SKU: "); // receive SKU of the item
     scanf("%d", &sku);
     flag = locateItem(item, NoOfRecs, sku, &i); // find the location

   	 if(flag != 1){
      printf("SKU not found in storage!\n");   
     }
     else{  // if find the item is in the inventory  
	  displayItem(item[i], FORM);  //display(FORM)
      printf("Please enter the quantity %s; Maximum of %d or 0 to abort: ", 
	  stock==STOCK ? "to stock" : "to checkout", stock==STOCK ? MAX_QTY - item[i].quantity : item[i].quantity);
      //if receive STOCK, Print "to stock" and quotient of item quantity subtracted from maximum quantity
	  
	  scanf("%d", &k);
	   if (k == 0){  // if decided not to stock
      	 printf("--== Aborted! ==--\n");  //print Aborted
		  return;	  
	  }
	  if(stock == STOCK){// this if for stock (accumulation)
		while((k > (MAX_QTY-item[i].quantity)) || (k < 0)){  // if the input is the out of proper range, ask again
	     printf("Please enter the quantity %s; Maximum of %d or 0 to abort: ", 
		 stock == STOCK? "to stock" : "to checkout", stock == STOCK? MAX_QTY-item[i].quantity : item[i].quantity);
	     scanf("%d", &k);
	    }
	 	item[i].quantity += k;  // if receive a proper number, add to original quantity
	 	printf("--== Stocked! ==--\n");

      }else if(stock == CHECKOUT){ // this if for check out (subtraction)
	   while((k > item[i].quantity) || (k < 0)){
         printf("Please enter the quantity %s; Maximum of %d or 0 to abort: ", 
		 stock==STOCK ? "to stock" : "to checkout", stock==STOCK ? MAX_QTY - item[i].quantity:item[i].quantity);
	     scanf("%d", &k);
	    }
	 	item[i].quantity -= k; // if receive a proper number, subtract.
	 	printf("--== Checked out! ==--\n");
      }
   }
   Low_or_not =isLowQuantity(item[i]); 
   if(Low_or_not == 1){  // if the quantity is lower than minimum
	printf("Quantity is low, please reorder ASAP!!!\n\n"); // show reorder warning
   }	
}


//ms4 functions here

void saveItem(struct Item item, FILE* dataFile){  // save item into a text file (write a file) 
fprintf(dataFile, "%d,%d,%d,%.2lf,%d,%s\n", item.sku, item.quantity, item.minQuantity, item.price, item.isTaxed, item.name); 
}
int loadItem(struct Item* item, FILE* dataFile){  // load data from a text file
int result=1; 
	if(dataFile!=NULL){
		fscanf(dataFile,"%d,%d,%d,%lf,%d,%20[^\n]",  // receive data from datafile
		&item->sku, &item->quantity, &item->minQuantity, &item->price, &item->isTaxed, item->name);
	}
	else{
	 printf("Error!Cannot open the file\n");  // fail to open the file
     result=0;
	}
return result;
}
int saveItems(const struct Item item[], char fileName[], int NoOfRecs){ // save items into a file from the struct
FILE* tp = fopen(fileName, "w");
int i;
int result=1;
		 
	if(tp == NULL){
		result=0;
	}
	else{
		 for(i=0 ; i < NoOfRecs; i++){
			fprintf(tp,"%d,%d,%d,%.2lf,%d,%s\n", 
			item[i].sku, item[i].quantity, item[i].minQuantity, item[i].price, item[i].isTaxed, item[i].name); 
		}
	   fclose(tp);
	}
return result;	
}
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr){ //load items from a datafile
FILE *tp;
int k = 0;
int i;
tp = fopen(fileName,"r");
   if(tp != NULL){
        while(fscanf(tp, "%d,%d,%d,%lf,%d,%20[^\n]", 
		&item[k].sku, &item[k].quantity, &item[k].minQuantity, &item[k].price, &item[k].isTaxed, item[k].name) > 0) {
        k++;
    }
	*NoOfRecsPtr = k;
    fclose(tp);
    return 1;
        }
	else
    return 0;
}

