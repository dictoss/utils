#include "StdAfx.h"
#include "DbDataReader.h"

/*****************************************************************************
 * �R���X�g���N�^
 *****************************************************************************/
DbDataReader::DbDataReader()
{
}

/*****************************************************************************
 * �f�X�g���N�^
 *****************************************************************************/
DbDataReader::~DbDataReader()
{
}


#if defined(_WIN32_WCE) || defined(_WIN32)

#if defined(_WIN64_WCE) || defined(_WIN64)
/* SQL Server Compact 3.5����*/
const ULONG  DbSqlCeDataReader::dblength_size = sizeof(ULONGLONG);
#else
/* SQL Server 2005 Compact�܂�*/
const ULONG DbSqlCeDataReader::dblength_size = sizeof(ULONG);
#endif

/*****************************************************************************
 * �R���X�g���N�^
 *****************************************************************************/
DbSqlCeDataReader::DbSqlCeDataReader() :
	m_IsInit(false), m_RowCount(0), m_ColCount(0),
	m_pRowsetPos(0), m_pMalloc(0), m_pRowset(0), m_pAccessor(0),
	m_pColInfo(0), m_pDbColInfo(0), m_pBind(0),
	m_hAccessor(0), m_pRowHeader(0),
	m_pRowData(0), m_ulBufferSize(0), m_bRowDataEnable(false)
{
	memset( m_hRows, 0, ARRAYSIZE(m_hRows) );
	m_pRowHeader = m_hRows;
}

//
DbSqlCeDataReader::DbSqlCeDataReader(IRowsetPosition *setpos) :
	m_IsInit(false), m_RowCount(0), m_ColCount(0),
	m_pRowsetPos(0), m_pMalloc(0), m_pRowset(0), m_pAccessor(0),
	m_pColInfo(0), m_pDbColInfo(0), m_pBind(0),
	m_hAccessor(0), m_pRowHeader(0),
	m_pRowData(0), m_ulBufferSize(0), m_bRowDataEnable(false)
{
	m_pRowsetPos = setpos;

	memset( m_hRows, 0, ARRAYSIZE(m_hRows) );
	m_pRowHeader = m_hRows;

	init();
}

/*****************************************************************************
 * �f�X�g���N�^
 *****************************************************************************/
DbSqlCeDataReader::~DbSqlCeDataReader()
{
	if( m_pRowData ){
		delete[] m_pRowData;
	}

	if( m_hRows[0] ){
		m_pRowset->ReleaseRows( 1, m_hRows, NULL, NULL, NULL );
	}
	if( m_hAccessor ){
		m_pAccessor->ReleaseAccessor( m_hAccessor, NULL );
	}

	if( m_pBind ){
		delete[] m_pBind;
	}

	if( m_pDbColInfo ){
		//m_pMalloc->Free( m_pColInfo );
	}

	if( m_pColInfo ){
		m_pColInfo->Release();
	}

	if( m_pAccessor ){
		m_pAccessor->Release();
	}

	if( m_pRowset ){
		m_pRowset->Release();
	}

	if( m_pMalloc ){
		m_pMalloc->Release();
	}

	if( m_pRowsetPos ){
		m_pRowsetPos->Release();
	}
}


/*****************************************************************************
 * ������
 *****************************************************************************/
bool DbSqlCeDataReader::init()
{
	HRESULT hr;
	ULONG i;
	OLECHAR *pStr = NULL;

	if( m_IsInit ){
		return true;
	}
	m_IsInit = false;

	if( NULL == m_pRowsetPos ){
		return false;
	}

	try{
		hr = m_pRowsetPos->QueryInterface( IID_IRowset, (void**)&m_pRowset );
		if( FAILED( hr ) )		throw(1);

		// �q�b�g�����������擾
		hr = m_pRowsetPos->GetRecordCount( DB_NULL_HCHAPTER, &m_RowCount );
		if( FAILED( hr ) )		throw(2);

		hr = m_pRowset->QueryInterface( IID_IColumnsInfo, (void**)&m_pColInfo );
		if( FAILED( hr ) )		throw(3);

		// �J�������̎擾
		hr = m_pColInfo->GetColumnInfo( &m_ColCount, &m_pDbColInfo, &pStr );
		if( FAILED( hr ) )		throw(3);

        // �擪[0]�ɂ͂Ȃɂ������Ă��邪�K�v�ȃJ�������͂Ȃ��̂ŃJ�E���g���܂���B
        // bookmark�Ƃ����炵���ł��B
        m_ColCount --;

		// �K�v�Ȃ��̂ŊJ���B
		CoTaskMemFree( pStr );

		// �f�[�^���O���̏ꍇ�̓f�[�^���Ȃ��̂Ŏ擾���������͂��܂���B
		if( 0 == m_RowCount ){
			m_IsInit = true;
			return m_IsInit;
		}

		hr = m_pRowset->QueryInterface( IID_IAccessor, (void**)&m_pAccessor );
		if( FAILED( hr ) )		throw(3);

		// �ŏ��̂P�͂���Ȃ��ł��B
		m_pBind = new DBBINDING[ m_ColCount ];
		if( NULL == m_pBind )		throw(3);

		memset( m_pBind, 0, sizeof(DBBINDING) * (m_ColCount) );

		// �o�C���h���܂��B
        ULONG offset = 0;
        DBCOLUMNINFO *pCurrentColInfo = m_pDbColInfo;

		for ( i = 0; i < m_ColCount; i++ ) {
            pCurrentColInfo = &(m_pDbColInfo[i + 1]);

            m_pBind[i].iOrdinal = pCurrentColInfo->iOrdinal;
			m_pBind[i].pTypeInfo = NULL;
			m_pBind[i].pObject = NULL;
			m_pBind[i].pBindExt = NULL;
			m_pBind[i].dwFlags = 0;
			m_pBind[i].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
			m_pBind[i].eParamIO = DBPARAMIO_OUTPUT;
			m_pBind[i].wType = pCurrentColInfo->wType;
			m_pBind[i].bPrecision = pCurrentColInfo->bPrecision;
			m_pBind[i].bScale = pCurrentColInfo->bScale;

			m_pBind[i].dwPart = DBPART_STATUS | DBPART_LENGTH | DBPART_VALUE;
			m_pBind[i].obStatus = offset;
			m_pBind[i].obLength = m_pBind[i].obStatus + sizeof(DBSTATUS);
			// SQL Server Compact�̃o�[�W�����ɂ���Ē�`���قȂ�܂��B
			//    ex) SQL Server 2005 Compact  sizeof(ULONG)
			//        SQL Server Compact 3.5 AND 64bit  sizeof(DBLENGTH) = ULONGLONG
			m_pBind[i].obValue = m_pBind[i].obLength + DbSqlCeDataReader::dblength_size;

            // �i�[�p�̃o�b�t�@�����v�Z�B
            switch( pCurrentColInfo->wType ){
                case DBTYPE_WSTR:
                case DBTYPE_BSTR:
                    // NULL������DBMS�������ĕԂ�����NULL�����̈悪�K�v�B
        			m_pBind[i].cbMaxLen = pCurrentColInfo->ulColumnSize * sizeof(WCHAR) + sizeof(WCHAR);
                    break;
                case DBTYPE_STR:
                    // NULL������DBMS�������ĕԂ�����NULL�����̈悪�K�v�B
                    m_pBind[i].cbMaxLen = pCurrentColInfo->ulColumnSize * sizeof(char) + sizeof(char);
                    break;
                default:
                   m_pBind[i].cbMaxLen = pCurrentColInfo->ulColumnSize;
                   break;
            }

            // ���̃I�t�Z�b�g�l���v�Z�B
            offset = m_pBind[i].obValue + m_pBind[i].cbMaxLen;

            // �������A���C�����g�����낦�܂��B(ARM���ƃG���[�ɂȂ邽�߁B)
            ULONG arignment = 8;
            offset += (arignment - (offset & (arignment-1)));
		};

		m_ulBufferSize = offset;

		// ���ʃZ�b�g���o�C���h���܂��B
		hr = m_pAccessor->CreateAccessor( DBACCESSOR_ROWDATA, m_ColCount,
				m_pBind, m_ulBufferSize, &m_hAccessor, NULL);
		if( FAILED( hr ) )		throw(3);

		// ���ʂ��i�[����o�b�t�@���m��
		m_pRowData = new BYTE[ m_ulBufferSize ];
		if( NULL == m_pRowData )		throw(3);

		// �����B
		m_IsInit = true;
	}
	catch( ... ){
		//
	}

	return m_IsInit;
}

bool DbSqlCeDataReader::init( IRowsetPosition *setpos )
{
	m_pRowsetPos = setpos;

	return init();
}

/*****************************************************************************
 * 
 *****************************************************************************/
DbDataReader::SEEK_RESULT DbSqlCeDataReader::read()
{
	return seek( 1 );
}

/*****************************************************************************
 * 
 *****************************************************************************/
DbDataReader::SEEK_RESULT DbSqlCeDataReader::prev_read()
{
	return seek( -1 );
}

/*****************************************************************************
 * 
 *****************************************************************************/
DbDataReader::SEEK_RESULT DbSqlCeDataReader::seek( int pos )
{
	int i;
	HRESULT hr;
	ULONG getRows = 0;
    int upordown;
    int count;

	if( 0 == m_RowCount ){
		return SEEK_EOF;
	}

    if( pos == 0 ){
		return SEEK_OK;
	}
    else if( 0 < pos ){
        upordown = 1;
        count = pos;
    }
    else{
        upordown = -1;
        count = -1 * pos;
    }

	try{
		// �ŏ��̍s�n���h�����擾�B
		for( i = 0; i < count; i ++ ){
			// �N���A���܂��B
			if( m_pRowData ){
				memset( m_pRowData, 0, m_ulBufferSize );
				m_bRowDataEnable = false;
			}

            if( m_hRows[0] ){
			    hr = m_pRowset->ReleaseRows( 1, m_pRowHeader, NULL, NULL, NULL );
                if( FAILED( hr ) ){
                    throw(1);
                }
                else{
                    m_hRows[0] = NULL;
                }
            }

			// �擾
			hr = m_pRowset->GetNextRows( DB_NULL_HCHAPTER, 0, upordown, &getRows, &m_pRowHeader);
			if( S_OK != hr ){
				if( DB_S_ENDOFROWSET == hr ){
					return SEEK_EOF;
				}
				else{
					throw(1);
				}
			}
		}
	}
	catch(...){
		return SEEK_ERROR;
	}

	return SEEK_OK;
}

/*****************************************************************************
 * 
 *****************************************************************************/
bool DbSqlCeDataReader::is_eof()
{
	return m_isEof;
}

/*****************************************************************************
 * 
 *****************************************************************************/
int DbSqlCeDataReader::get_column( unsigned int index, void *buf, size_t size, int type )
{
	HRESULT hr;

	if( (index < 0) || ((m_ColCount-1) < index) ){
		return -1;		// �J�����ʒu�I�[�o�[�B
	}

	if( false == m_bRowDataEnable ){
		// �P�s���̃f�[�^���擾
		hr = m_pRowset->GetData( m_hRows[0], m_hAccessor, m_pRowData );
		if( FAILED( hr ) ){
			return -1;
		}

		m_bRowDataEnable = true;
	}

	// �J����������o���܂��B
	switch( m_pDbColInfo[index + 1].wType ){
	case DBTYPE_WSTR:
	case DBTYPE_BSTR:
        {
            WCHAR *pDest = (WCHAR*)buf;
            WCHAR *pSrc  = (WCHAR*)(m_pRowData + m_pBind[index].obValue);
            ULONG colsize = m_pDbColInfo[index + 1].ulColumnSize * sizeof(WCHAR) + sizeof(WCHAR);

            if( size < colsize ){
                // �o�b�t�@���������̂Ő؂�l�߂܂��B
                wcsncpy( pDest, pSrc, size / sizeof(WCHAR) );
                pDest[size / sizeof(WCHAR) - 1] = 0x00;
            }
            else{
                wcscpy( pDest, pSrc );
            }

            pDest = pDest;
        }
		break;
	case DBTYPE_STR:
        {
            char *pDest = (char*)buf;
            char *pSrc  = (char*)(m_pRowData + m_pBind[index].obValue);
            ULONG colsize = m_pDbColInfo[index + 1].ulColumnSize * sizeof(char) + sizeof(char);

            if( size < colsize ){
                // �o�b�t�@���������̂Ő؂�l�߂܂��B
                strncpy( pDest, pSrc, size / sizeof(char) );
                pDest[size / sizeof(char) - 1] = 0x00;
            }
            else{
                strcpy( pDest, pSrc );
            }

            pDest = pDest;
        }
		break;
	case DBTYPE_I4:
		*((int*)buf) = *((int*)(m_pRowData + m_pBind[index].obValue));
		break;
	default:
		return -1;
		break;
	}

	return 0;
}
#endif /* defined(_WIN32_WCE) || defined(_WIN32) */
