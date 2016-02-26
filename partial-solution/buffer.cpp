/* ----------------------------------------------------------------------------
 * Copyright &copy; 2016 Alan_Reeve <areeve.nos@gmail.com>
 * Released under the [MIT License] (http://opensource.org/licenses/MIT)
 * ------------------------------------------------------------------------- */

/**
 * Implements `buffer.h`.
 */

// TODO: `#include`s for system headers, if necessary

#include "buffer.h"
#include <iostream>
#include <cstdlib>
 
// TODO: `#include`s for other local headers, if necessary

using std::cout;
using std::string;
using std::cerr;

// ----------------------------------------------------------------------------

// TODO: implementations for all functions in `class Buffer`

    Buffer::Buffer(unsigned int size_x, unsigned int size_y) : size_x(size_x), size_y(size_y){
        data_ = new char[size_x * size_y];
        
        Buffer::clear();    
    }   

    Buffer::~Buffer(){

        delete[] data_;
    
    }

    char Buffer::get(unsigned int x, unsigned int y) const{
       
        if (x >= size_x || y >= size_y){
            cerr << "Error: 'Buffer::get': index out of bounds\n";
            exit(1);
        }

        return data_[ y * size_x + x];
        
    }

    void Buffer::set(unsigned int x, unsigned int y, char c){
        
        if (x >= size_x || y >= size_y){
            cerr << "Error: 'Buffer::set': index out of bounds\n";
            cout << "x: " << x << "\ny: " << y << "\n";
            exit(1);
        }

        data_[y * size_x + x] = c;

    }

    void Buffer::set(unsigned int pos_x, unsigned int pos_y, string s){
        
        for (int i = 0, x=pos_x, y=pos_y; i < s.length(); i++){

            if(s[i] == '\n'){
                y++;
                x=pos_x;
            }
            else{
                set(x++, y, s[i]);
            }
         }

    }

    void Buffer::draw() const{
        for(int y = 0; y < size_y; y++){
            
            for(int x = 0; x < size_x; x++){
                
                cout << get(x,y);
            }
            cout << std::endl;
        }

    }

    void Buffer::clear(){

        for (int i =0; i < (size_x * size_y); i++){
            data_[i] = ' ';
        }

    }
    

    
