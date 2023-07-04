#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buf_() {}

void Writer::push( string data )
{
  // Your code here.
  //(void)data;
  uint64_t size = min( data.size(), capacity_ - buf_.size() );
  data = data.substr( 0, size );
  for ( const auto& c : data ) {
    buf_.push_back( c );
  }
  write_tol += size;
}

void Writer::close()
{
  flag_close = true;
  // Your code here.
}

void Writer::set_error()
{
  // Your code here.
  flag_error = true;
}

bool Writer::is_closed() const
{
  // Your code here.
  return flag_close;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - buf_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return write_tol;
}

string_view Reader::peek() const
{
  // Your code here.
  // std::string_view res=string_view(&buf_.front(),1);
  return { std::string_view( &buf_.front(), 1 ) };
}

bool Reader::is_finished() const
{
  // Your code here.
  return buf_.empty() && flag_close;
}

bool Reader::has_error() const
{
  // Your code here.
  return flag_error;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  // (void)len;
  const uint64_t size = min( len, buf_.size() );
  buf_.erase( buf_.begin(), buf_.begin() + size );
  read_tol += size;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buf_.size();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return read_tol;
}
