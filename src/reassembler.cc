#include "reassembler.hh"
#include "iostream"
using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  // Your code here.
  process_substr(first_index,data,output);
  if(first_index==next_index){
    std::string next_string=unorder_buf[first_index];
    output.push(next_string);
    unorder_bytes_size-=next_string.size();
    unorder_buf.erase(first_index);
  }
  // std::cout<<output.bytes_pushed()<<std::endl;
  if ( is_last_substring ) 
  {
    byte_stream_end = true;
    eof_index = first_index + data.size();
  }
  if ( byte_stream_end && output.bytes_pushed() == eof_index ) // all bytes pushed
  {
    output.close();
  }

}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return unorder_bytes_size;
}

void Reassembler::process_substr( uint64_t& first_index, std::string& data, Writer& output )
{
  if ( data.size() == 0 ) {
    return;
  }
  size_t first_unassembled_index = output.bytes_pushed();
  size_t first_unacceptable_index = first_unassembled_index + output.available_capacity();
  size_t data_last_index = first_index + data.size() - 1;
  next_index = first_unassembled_index;
  // 缓冲区域不足
  if ( first_unassembled_index >= first_unacceptable_index ) {
    return;
  }
  // 起始位置不对
  if ( first_index >= first_unacceptable_index || data_last_index < first_unassembled_index ) {
    return;
  } else if ( first_index < first_unassembled_index && data_last_index >= first_index ) {
    data = data.substr( first_unassembled_index - first_index );
    first_index = first_unassembled_index;
  } else if ( first_index >= first_unassembled_index && data_last_index >= first_unacceptable_index ) {
    data = data.substr( 0, first_unacceptable_index - first_index );
  }
  remove_overlap( first_index, data );
  unorder_buf[first_index] = data;
  unorder_bytes_size += data.size();
  return;
}

void Reassembler::remove_overlap( uint64_t& first_index, std::string& data )
{
  if ( unorder_buf.size() == 0 ) {
    return;
  }
  for ( auto iter = unorder_buf.begin(); iter != unorder_buf.end(); ) {
    size_t last_index = first_index + data.size() - 1;
    size_t begin_index = iter->first;
    size_t end_index = begin_index + iter->second.size() - 1;
    if ( last_index + 1 == begin_index ) {
      data += iter->second;
      unorder_bytes_size -= iter->second.size();
      iter = unorder_buf.erase( iter );
    } else if ( first_index <= begin_index && begin_index <= last_index ) {
      if ( end_index <= last_index ) {
        unorder_bytes_size -= unorder_buf[begin_index].size();
        iter = unorder_buf.erase( iter );
      } else {
        data += iter->second.substr( last_index - begin_index + 1 );
        unorder_bytes_size -= unorder_buf[begin_index].size();
        iter = unorder_buf.erase( iter );
      }
    } else if ( first_index >= begin_index && first_index <= end_index ) {
      if ( last_index <= end_index ) {
        first_index = begin_index;
        data = iter->second;
        unorder_bytes_size -= iter->second.size();
        iter = unorder_buf.erase( iter );
      } else {
        data=iter->second.substr(0,first_index-begin_index)+data;
        unorder_bytes_size -= iter->second.size();
        iter = unorder_buf.erase( iter );
        first_index=begin_index;
      }
    }else{
      iter++;
    }
  }
}