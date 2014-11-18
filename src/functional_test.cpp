#include "publisher.hpp"
#include "subscriber.hpp"
#include <cassert>
#include <iostream>

int main()
{
    publisher< int > p0( 16 );
    subscriber< int >& s0 = p0.subscribe();
    subscriber< int >& s1 = s0.subscribe();

    std::cout << "p0 available: " << p0.available() << std::endl;
    std::cout << "s0 available: " << s0.available() << std::endl;
    std::cout << "s1 available: " << s1.available() << std::endl;
    std::cout << std::endl;

    size_t available = p0.available();
    for( int i=0; i<available; i++ ) {
        p0.at( i ) = i;
    }
    p0.commit( available );

    std::cout << "p0 available: " << p0.available() << std::endl;
    std::cout << "s0 available: " << s0.available() << std::endl;
    std::cout << "s1 available: " << s1.available() << std::endl;
    std::cout << std::endl;

    available = s0.available();
    for( int i=0; i<available; i++ ) {
        assert( s0.at( i ) == i );
    }
    s0.commit( available );

    std::cout << "p0 available: " << p0.available() << std::endl;
    std::cout << "s0 available: " << s0.available() << std::endl;
    std::cout << "s1 available: " << s1.available() << std::endl;
    std::cout << std::endl;

    available = s1.available();
    for( int i=0; i<available; i++ ) {
        assert( s1.at( i ) == i );
    }
    s1.commit( available );

    std::cout << "p0 available: " << p0.available() << std::endl;
    std::cout << "s0 available: " << s0.available() << std::endl;
    std::cout << "s1 available: " << s1.available() << std::endl;
    std::cout << std::endl;
}