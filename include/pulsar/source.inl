template< class T >
inline source< T >::source( size_t n ) :
    _queue( n ),
    _head( 0 )
{
}

template< class T >
inline size_t source< T >::available()
{
    // apply memory barrier to ensure all positions are correct
    std::atomic_thread_fence( std::memory_order::memory_order_acquire );

    // find the slowest subscription
    // only consider subscriptions that are still alive
    // remove dead subscriptions
    position tail_min = std::numeric_limits< position >::max();
    for( auto it = _tail.begin(); it != _tail.end(); )
    {
        if( (*it)->_alive ) {
            tail_min = std::min( (*it)->_tail, tail_min ); it++;
        } else {
            it = _tail.erase( it );
        }
    }

    // number of slots available is head - min( tail )
    // this ensures we can never write past the slowest subscription
    // if there are no subscriptions we cannot publish
    if( _tail.size() ) {
        return _queue.size() - ( _head - tail_min );
    } else {
        return 0;
    }
}

template< class T >
inline size_t source< T >::commit( size_t n )
{
    // issue a memory barrier to ensure the queue is consistent
    // across threads then increment head
    std::atomic_thread_fence( std::memory_order::memory_order_release );
    _head += n;
    return n;
}

template< class T >
inline T& source< T >::at( size_t i )
{
    return _queue.at( _head + i );
}

template< class T >
inline subscription< T >& source< T >::subscribe()
{
    _tail.push_back( std::unique_ptr< subscription< T > >( new subscription< T >( *this, _head ) ) );
    return *_tail.back();
}

template< class T >
inline subscription< T >& source< T >::subscribe( position& h )
{
    _tail.push_back( std::unique_ptr< subscription< T > >( new subscription< T >( *this, h ) ) );
    return *_tail.back();
}