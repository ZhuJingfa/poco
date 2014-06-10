//
// PostgreSQLTypes.h
//
// $Id: //poco/1.4/Data/PostgreSQL/include/Poco/Data/PostgreSQL/SessionHandle.h#1 $
//
// Library: Data
// Package: PostgreSQL
//
// Definition of the SessionHandle class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef Data_PostgreSQL_Types_INCLUDED
#define Data_PostgreSQL_Types_INCLUDED

#include <vector>

#include <libpq-fe.h>

namespace Poco {
namespace Data {
namespace PostgreSQL {

enum PostgreSQLSupportedFieldTypes
	/// Enum to convey type information between Poco/C++ data types and PostgreSQL
{
    POSTGRESQL_TYPE_NONE,
    POSTGRESQL_TYPE_INT8,
    POSTGRESQL_TYPE_UINT8,
    POSTGRESQL_TYPE_INT16,
    POSTGRESQL_TYPE_UINT16,
    POSTGRESQL_TYPE_INT32,
    POSTGRESQL_TYPE_UINT32,
    POSTGRESQL_TYPE_INT64,
    POSTGRESQL_TYPE_UINT64,
    POSTGRESQL_TYPE_BOOLEAN,
    POSTGRESQL_TYPE_FLOAT,
    POSTGRESQL_TYPE_DOUBLE,
    POSTGRESQL_TYPE_CHAR,
    POSTGRESQL_TYPE_STRING,
    POSTGRESQL_TYPE_BLOB,
    POSTGRESQL_TYPE_CLOB,
    POSTGRESQL_TYPE_DATETIME,
    POSTGRESQL_TYPE_DATE,
    POSTGRESQL_TYPE_TIME,
    POSTGRESQL_TYPE_NULL
};

class InputParameter
	/// PostgreSQL class to record values for input parameters to SQL statements
{
public:
    explicit InputParameter( PostgreSQLSupportedFieldTypes aFieldType,
                             const void *                  aDataPtr,
                             std::size_t                   theDataLength );
    explicit InputParameter();
    
    ~InputParameter();
    
    PostgreSQLSupportedFieldTypes   fieldType() const;
    const void *                    pData() const;
    std::size_t                     size() const;
    bool                            isBinary() const;
    
    void                            setStringVersionRepresentation( const std::string & aString );
    void                            setNonStringVersionRepresentation( const void * aPtr, std::size_t theSize );
    
    const void *                    pInternalRepresentation() const;

private:
    PostgreSQLSupportedFieldTypes   _fieldType;
    const void *                    _pData;
    std::size_t                     _size;
    bool                            _isBinary;
    std::string                     _stringVersionRepresentation;
    void *                          _pNonStringVersionRepresentation;
};

typedef std::vector < InputParameter > InputParameterVector;


class OutputParameter
	/// PostgreSQL class to record values for output parameters to capture the results
{
public:
    explicit OutputParameter( PostgreSQLSupportedFieldTypes aFieldType,
                              Oid                           anInternalFieldType,
                              std::size_t                   aRowNumber,
                              const char *                  aDataPtr,
                              std::size_t                   theSize,
                              bool                          anIsNull );
    
    explicit OutputParameter();
    
    ~OutputParameter();
    
    void                            setValues( PostgreSQLSupportedFieldTypes aFieldType,
                                               Oid                           anInternalFieldType,
                                               std::size_t                   aRowNumber,
                                               const char *                  aDataPtr,
                                               std::size_t                   theSize,
                                               bool                          anIsNull);
    
    PostgreSQLSupportedFieldTypes   fieldType() const;
    Oid                             internalFieldType() const;
    std::size_t                     rowNumber() const;
    const char *                    pData() const;
    std::size_t                     size() const;
    bool                            isNull() const;

private:
    PostgreSQLSupportedFieldTypes   _fieldType;
    Oid                             _internalFieldType;
    std::size_t                     _rowNumber;
    const char *                    _pData;
    std::size_t                     _size;
    bool                            _isNull;
};

typedef std::vector < OutputParameter > OutputParameterVector;


class PQConnectionInfoOptionsFree
	/// PostgreSQL connection Info Options free (RAII)
{
public:
    explicit PQConnectionInfoOptionsFree( PQconninfoOption * aConnectionInfoOptionPtr );
    ~PQConnectionInfoOptionsFree();
        
private:
    PQConnectionInfoOptionsFree             ( const PQConnectionInfoOptionsFree & );
    PQConnectionInfoOptionsFree & operator= ( const PQConnectionInfoOptionsFree & );
        
private:
    PQconninfoOption * _pConnectionInfoOption;
};

class PQResultClear
	/// PostgreSQL statement result free (RAII)
{
public:
    explicit PQResultClear( PGresult * aPQResultPtr );
    ~PQResultClear();
        
private:
    PQResultClear             ( const PQResultClear & );
    PQResultClear & operator= ( const PQResultClear & );
        
private:
    PGresult * _pPQResult;
};
  

class PGCancelFree
	/// PostgreSQL Cancel Info Options free (RAII)
{
public:
    explicit PGCancelFree( PGcancel * aStatementCancelPtr );
    ~PGCancelFree();
        
private:
    PGCancelFree             ( const PGCancelFree & );
    PGCancelFree & operator= ( const PGCancelFree & );
        
private:
    PGcancel * _pPGCancel;
};
    
    
//
// inlines
//

// InputParameter

inline
InputParameter::InputParameter( PostgreSQLSupportedFieldTypes aFieldType,
                                const void *                  aDataPtr,
                                std::size_t                   theSize )
    : _fieldType                       ( aFieldType ),
      _pData                           ( aDataPtr ),
      _size                            ( theSize ),
      _isBinary                        ( false ),
      _pNonStringVersionRepresentation ( 0 )
{
    if (    POSTGRESQL_TYPE_BLOB == _fieldType
         || POSTGRESQL_TYPE_CLOB == _fieldType) {
        _isBinary = true;
    }
}


inline
InputParameter::InputParameter()
    : _fieldType                      ( POSTGRESQL_TYPE_NONE ),
      _pData                          ( 0 ),
      _size                           ( 0 ),
      _isBinary                       ( false ),
     _pNonStringVersionRepresentation ( 0 )
{
}


inline
InputParameter::~InputParameter()
{
}


inline
const void *
InputParameter::pData() const
{
    return _pData;
}


inline
PostgreSQLSupportedFieldTypes
InputParameter::fieldType() const
{
    return _fieldType;
}


inline
std::size_t
InputParameter::size() const
{
    return _size;
}


inline
bool
InputParameter::isBinary() const
{
    return _isBinary;
}

inline
void
InputParameter::setStringVersionRepresentation( const std::string & aString )
{
    _pNonStringVersionRepresentation = 0;
    
    _stringVersionRepresentation = aString;
    _size = _stringVersionRepresentation.size();
}

inline
void
InputParameter::setNonStringVersionRepresentation( const void * aPtr, std::size_t theDataLength )
{
    _stringVersionRepresentation = std::string();
    
    _pNonStringVersionRepresentation = const_cast< void * > ( aPtr );
    _size = theDataLength;
}


inline
const void *
InputParameter::pInternalRepresentation() const
{
    switch ( _fieldType ) {
        case POSTGRESQL_TYPE_INT8:
        case POSTGRESQL_TYPE_UINT8:
        case POSTGRESQL_TYPE_INT16:
        case POSTGRESQL_TYPE_UINT16:
        case POSTGRESQL_TYPE_INT32:
        case POSTGRESQL_TYPE_UINT32:
        case POSTGRESQL_TYPE_INT64:
        case POSTGRESQL_TYPE_UINT64:
        case POSTGRESQL_TYPE_BOOLEAN:
        case POSTGRESQL_TYPE_FLOAT:
        case POSTGRESQL_TYPE_DOUBLE:
        case POSTGRESQL_TYPE_CHAR:
        case POSTGRESQL_TYPE_STRING:
        case POSTGRESQL_TYPE_DATETIME:
        case POSTGRESQL_TYPE_DATE:
        case POSTGRESQL_TYPE_TIME:
            return _stringVersionRepresentation.c_str();

        case POSTGRESQL_TYPE_BLOB:
        case POSTGRESQL_TYPE_CLOB:
            return _pNonStringVersionRepresentation;

        case POSTGRESQL_TYPE_NONE:
        case POSTGRESQL_TYPE_NULL:
        default:
            return 0;
    }
}

// OutputParameter
inline
OutputParameter::OutputParameter( PostgreSQLSupportedFieldTypes aFieldType,
                                  Oid                           anInternalFieldType,
                                  std::size_t                   aRowNumber,
                                  const char *                  aDataPtr,
                                  std::size_t                   theSize,
                                  bool                          anIsNull )
    : _fieldType         ( aFieldType ),
      _internalFieldType ( anInternalFieldType ),
      _rowNumber         ( aRowNumber ),
      _pData             ( aDataPtr ),
      _size              ( theSize ),
      _isNull            ( anIsNull )
{
}

inline
OutputParameter::OutputParameter( )
    : _fieldType         ( POSTGRESQL_TYPE_NONE ),
      _internalFieldType ( -1 ),
      _rowNumber         ( 0 ),
      _pData             ( 0 ),
      _size              ( 0 ),
      _isNull            ( true )
{
}

inline
OutputParameter::~OutputParameter( )
{
}

inline
void
OutputParameter::setValues( PostgreSQLSupportedFieldTypes aFieldType,
                            Oid                           anInternalFieldType,
                            std::size_t                   aRowNumber,
                            const char *                  aDataPtr,
                            std::size_t                   theSize,
                            bool                          anIsNull )
{
    _fieldType         = aFieldType;
    _internalFieldType = anInternalFieldType;
    _rowNumber         = aRowNumber;
    _pData             = aDataPtr;
    _size              = theSize;
    _isNull            = anIsNull;
}


inline
PostgreSQLSupportedFieldTypes
OutputParameter::fieldType() const
{
    return _fieldType;
}

inline
Oid
OutputParameter::internalFieldType() const
{
    return _internalFieldType;
}

inline
std::size_t
OutputParameter::rowNumber() const
{
    return _rowNumber;
}

inline
const char *
OutputParameter::pData() const
{
    return _pData;
}

inline
std::size_t
OutputParameter::size() const
{
    return _size;
}
    
inline
bool
OutputParameter::isNull() const
{
    return _isNull;
}

// PQConnectionInfoOptionsFree

inline
PQConnectionInfoOptionsFree::PQConnectionInfoOptionsFree( PQconninfoOption * aConnectionInfoOptionPtr )
    : _pConnectionInfoOption( aConnectionInfoOptionPtr )
{
}

inline
PQConnectionInfoOptionsFree::~PQConnectionInfoOptionsFree()
{
    if ( _pConnectionInfoOption )
    {
        PQconninfoFree( _pConnectionInfoOption );
        _pConnectionInfoOption = 0;
    }
}

// PQResultClear

inline
PQResultClear::PQResultClear( PGresult * aPQResultPtr  )
    : _pPQResult( aPQResultPtr )
{
}

inline
PQResultClear::~PQResultClear()
{
    if ( _pPQResult )
    {
        PQclear( _pPQResult );
        _pPQResult = 0;
    }
}

// PGCancelFree

inline
PGCancelFree::PGCancelFree( PGcancel * aStatementCancelPtr  )
    : _pPGCancel( aStatementCancelPtr )
{
}

inline
PGCancelFree::~PGCancelFree()
{
    if ( _pPGCancel )
    {
        PQfreeCancel( _pPGCancel );
        _pPGCancel = 0;
    }
}
    
    
}}}

#endif // Data_PostgreSQL_Types_INCLUDED
