/* ----------------------------------------------------------------------------
 * Copyright &copy; 2016 Alan Reeve <areeve@csu.fullerton.edu>
 * Released under the [MIT License] (http://opensource.org/licenses/MIT)
 * ------------------------------------------------------------------------- */


#include <iostream>
#include <cstdlib>
#include <string>

#include "constants.h"
#include "shape.h"
using std::string;
using std::cerr;
using std::cout;
// ----------------------------------------------------------------------------


Shape::Shape(const string &type) {    
    setType(type);
}

Shape::~Shape() = default;

string Shape::getType(){
    return type_;
}

void Shape::setType(const string &type){
    
    if (type != "triangle" && type != "square" && type != "empty"){
        cerr << "ERROR: 'Shape::setType': invalid type\n";
        exit(1); // error
    }
      
    type_ = type;
    
    if(type == "triangle")
       type_art_ = "  ,  \n / \\ \n/___\\\n"; 

    if(type == "square")
        type_art_ = ".---.\n|   |\n'---'\n";

    if(type == "empty")
        type_art_ = "     \n     \n     \n"; 
}
   


void Shape::drawToBuffer(Buffer &b, unsigned int x, unsigned int y) const{
    b.set(x, y, type_art_);       
}

bool Shape::isHappy( const Neighborhood &n,
                  unsigned int pos_x,
                  unsigned int pos_y) const{
	
	if (n.get(pos_x, pos_y).getType() == "empty")
    	return true;

    // find the min and max coordinates of possible neighbors
	unsigned int x_min = (pos_x == 0) ? pos_x : pos_x - 1;
	unsigned int y_min = (pos_y == 0) ? pos_y : pos_y - 1;
	unsigned int x_max = (pos_x == n.size_x-1) ? pos_x : pos_x + 1;
	unsigned int y_max = (pos_y == n.size_y-1) ? pos_y : pos_y + 1;

	double alike = 0;
	double different = 0;

	// evaluate each neighbor to deteremine likeness
	for (int x=x_min; x <= x_max; x++) {
    	for (int y=y_min; y <= y_max; y++) {
       		if (x == pos_x && y == pos_y)
            	continue;
        	else if (n.get(x, y).getType() == "empty")
            	continue;
        	else if (n.get(x, y).getType() == n.get(pos_x, pos_y).getType())
            	alike++;
        	else 
            	different++;
    	}
	}

	// returns true if the shape is happy
	return    ( different || alike )
       	&& ( different == 0 || alike / different >= RATIO_ALIKE_HAPPY )
       	&& ( alike == 0 || different / alike >= RATIO_DIFFERENT_HAPPY); 
}
        