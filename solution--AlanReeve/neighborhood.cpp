/* ----------------------------------------------------------------------------
 * Copyright &copy; 2016 Alan Reeve <areeve@csu.fullerton.edu>
 * Released under the [MIT License] (http://opensource.org/licenses/MIT)
 * ------------------------------------------------------------------------- */

#include <random>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

#include "neighborhood.h"
#include "constants.h"

// ----------------------------------------------------------------------------
// local helper functions

/**
 * Notes:
 * - If you like, you can start reading about what this "unnamed `namespace`"
 *   is for [here](http://www.comeaucomputing.com/techtalk/#nostatic).
 */
namespace {

    /**
     * Return a uniformly distributed random number in the range [low, high]
     * using the Mersenne Twister engine, seeded with (ideally) the hardware
     * random device.
     *
     * Because `rand()` (and especially `rand() % ...`) maybe shouldn't be
     * used, if we can avoid it:
     * [`rand()` considered harmful]
     * (https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful)
     * (talk by stephan t. lavavej)
     * 
     *
     * Notes:
     * - In C++, `static` function variables are created and initialized once,
     *   when the function is first called, and they remain until the program
     *   is terminated: 
     *   [Static variables in class methods]
     *   (http://stackoverflow.com/questions/6223355/static-variables-in-class-methods)
     * - `std::mt19937` objects are expensive to create.
     *   `std::uniform_int_distribution` objects are said to be pretty cheap.
     */
    unsigned int mtrand(unsigned int low, unsigned int high) {
        static std::random_device rd;
        static std::mt19937 mt(rd());  // engine (mersenne twister)

        return std::uniform_int_distribution<unsigned int>(low, high)(mt);
    }

}

// ----------------------------------------------------------------------------

Neighborhood::Neighborhood(unsigned int size_x, unsigned int size_y) 
    : size_x(size_x), size_y(size_y){

   	// initialize 'neighborhood_' and fill with "empty" shapes
    neighborhood_ = new Shape[size_x * size_y];

    for(int i = 0; i < (size_x * size_y); i++)
        neighborhood_[i].setType("empty");
    
    // fill with non-empty shapes so that the ratio of non-empty to empty
    // shapes is 'RATIO_FILLED' (see 'constants.h')
    for(int i=0, j=128; i < (j * RATIO_FILLED); i++){
    	int x = mtrand(0, size_x - 1);
    	int y = mtrand(0, size_y - 1);

    	if(this -> get(x, y).getType() == "empty"){
       		if( mtrand(0,1) == 0)
        		neighborhood_[i].setType("triangle");
        	else{
        		neighborhood_[i].setType("square");
        	}
        	j--;
   		}
    }    
}

Neighborhood::~Neighborhood(){
    delete[] neighborhood_;
}

Shape Neighborhood::get(unsigned int x, unsigned int y) const{

    if( x >= TERM_SIZE_X || y >= TERM_SIZE_Y ){
        std::cerr << "ERROR: `Neighborhood::get`: index out of bounds\n";
        exit(1);
    }
    return neighborhood_[y * size_x + x];
}

void Neighborhood::set(unsigned int x, unsigned int y, const Shape &s){
    if( x >= size_x || y >= size_y ){
        std::cerr << "ERROR: `Neighborhood::set`: index out of bounds\n";
        exit(1); // error
    }
    neighborhood_[ y * size_x + x] = s;
}

void Neighborhood::move(unsigned int old_x, unsigned int old_y){  
    int new_x, new_y;

    do{
        new_x = mtrand(0, size_x-1);
        new_y = mtrand(0, size_y-1);
    } while(get(new_x, new_y).getType() != "empty");
    
    set(new_x, new_y, get(old_x, old_y));
    set(old_x, old_y, Shape("empty"));
}

void Neighborhood::animate(unsigned int frames){ 
    // instantiate a buffer
    Buffer b(size_x * Shape::size_x, size_y * Shape::size_y);

    for(int f = 0; f < frames; f++){
        // draw all the shapes in our neighborhood to it
        for(int y = 0, i = 0; y < size_y; y ++){
        
            for(int x = 0; x < size_x; x++, i++){               
                neighborhood_[i].drawToBuffer(b, 
                        x * Shape::size_x, 
                        y * Shape::size_y);          
            }     
        }      
        // render it to the screen
        b.draw();

        // move all the unhappy shapes
        for(int y = 0, i = 0; y < size_y; y++)      
            for(int x = 0; x < size_x; x++, i++)              
                if(neighborhood_[i].getType() != "empty")
                    if(neighborhood_[i].isHappy(*this, x, y) == false)
                        move(x, y);                 
 
        // wait a little before drawing the next frame
        std::this_thread::sleep_for(std::chrono::milliseconds(100));   
    }
}
