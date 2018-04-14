
void func1(){

	static int recur = 1;

	recur++;
	func1();
}

void func3(){

	int x = 1;
	int y = 2;
	int z = 3;

	if(x == 1){
		
		z = 1000;
	}

	else {

		if(y == 1){
			
			z = 2000;
		}
		else {

			z = 3000;
		}
	}
}


int main(){

	int a = 1;
	int b = 2;
	int c = 3;

	if(a == 1){
		
		if(b == 1){

			c = 100;
			func1();
			func3();
		}
		else {
 
			c = 200;
			func1();
		}

	}

	else {

		c = 300;

		func3();
	}

}
