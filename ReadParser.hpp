#ifndef _MOURISL_READ_PARSER
#define _MOURISL_READ_PARSER

#include <vector>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  FORMAT_READ1,
  FORMAT_READ2,
  FORMAT_BARCODE,
  FORMAT_UMI,
  FORMAT_CATEGORY_COUNT
} ;

struct _segInfo
{
  int start ;
  int end ;
  int strand ; // -1: minus, 1:posiive

	bool operator<( const struct _segInfo &b )	const
  {
    return start < b.start ; 
  }
} ;

// Parse for read1, read2, barcode and UMI.
// Current implementation is not thread-safe.
class ReadParser
{
private:
  char *_buffer ;
  int _bufferSize ;
  std::vector<struct _segInfo> _segs[FORMAT_CATEGORY_COUNT] ;
  
  // Return false if it fails to parse the format string.
  bool ParseFormatStringAndAppendEffectiveRange(const char *s, int len) {
    int i;
    int j = 0;  // start, end, strand section
    char buffer[20];
    int blen = 0;
    
    struct _segInfo seg ;
    if (s[2] != ':')
      return false ;
    int category = 0 ;
    if (s[0] == 'r' && s[1] == '1') {
      category = FORMAT_READ1 ;
    } else if (s[0] == 'r' && s[1] == '2') {
      category = FORMAT_READ2 ;
    } else if (s[0] == 'b' && s[1] == 'c') {
      category = FORMAT_BARCODE ;
    } else if (s[0] == 'u' && s[1] == 'm') {
      category = FORMAT_UMI ;
    } else {
      return false ;
    }
    
    seg.strand = 1 ;
    for (i = 3; i <= len; ++i) {
      if (i == len || s[i] == ':') {
        buffer[blen] = '\0';
        if (j == 0) {
          seg.start = atoi(buffer) ;
        } else if (j == 1) {
          seg.end = atoi(buffer) ;
        } else {
          seg.strand = (buffer[0] == '+' ? 1 : -1);
        }

        blen = 0;
        if (i < len && s[i] == ':') {
          ++j;
        }
      } else {
        buffer[blen] = s[i];
        ++blen;
      }
    }

    if (j >= 3 || j < 1) {
      return false;
    }
    
    _segs[category].push_back(seg) ;
    return true;
  }
  
public:
  ReadParser() {
    _buffer = NULL;
    _bufferSize = 0;
  } 

  ~ReadParser() {
    if (_buffer != NULL)
      free(_buffer);
  }

  void Init(const char *formatStr) {
    int i, j;
    for (i = 0 ; formatStr[i] ; ) {
      for (j = i ; formatStr[j] && formatStr[j] != ';' && formatStr[j] != ',' ; ++j)
        ;
      
      if (!ParseFormatStringAndAppendEffectiveRange(formatStr + i, j - i))
      {
        fprintf(stderr, "Format description error in %s", formatStr) ;
        exit(1) ;
      }

      if (formatStr[j])
        i = j + 1 ;
      else
        i = j ;
    }
    
    // Sort the order in each specification
    for (i = 0 ; i < FORMAT_CATEGORY_COUNT ; ++i)
      std::sort(_segs[i].begin(), _segs[i].end()) ;
  }

  int NeedExtract(int category, int end)
  {
    int size = _segs[category].size() ;
    if (size == 0)
      return 0 ;
    else if (size == 1)
    {
      if (_segs[category][0].start == 0  
          && (_segs[category][0].end == -1 || _segs[category][0].end == end)
          && _segs[category][0].strand == 1)
        return 0 ;
    }
    else
      return 1 ;
  }

  // needComplement=true: reverse complement. Otherwise, just reverse
  const char* ExtractSequence(const char *seq, int category, bool needComplement)
  {
    int len = strlen(seq) ;
    if (!NeedExtract(category, len - 1))
      return seq ;
    if (len + 1 > _bufferSize)
    {
      _buffer = (char *)realloc(_buffer, len + 1) ;
      _bufferSize = len + 1 ; 
    }
    int i, j, k ;
    const std::vector<_segInfo> &seg = _segs[category] ;
    int segSize = seg.size() ;
    int strand = 1 ;
    i = 0 ;
    for (k = 0 ; k < segSize ; ++k)
    {
      for (j = seg[k].start ; j <= seg[k].end ; ++j)
      {
        _buffer[i] = seq[j] ;
        ++i ;
      }
    }
    _buffer[i] = '\0' ;
    return _buffer ;
  }
} ;

#endif
