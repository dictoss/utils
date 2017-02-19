/*****************************************************************************
 * データベースへのクライアントクラス
 *****************************************************************************/
#include "StdAfx.h"
#include "DbClient.h"
#include "DbDataReader.h"

/*****************************************************************************
 * コンストラクタ
 *****************************************************************************/
DbClient::DbClient()
{
}


/*****************************************************************************
 * デストラクタ
 *****************************************************************************/
DbClient::~DbClient()
{
}


#if defined(_WIN32_WCE) || defined(_WIN32)
/*****************************************************************************
 * スタティック変数
 *****************************************************************************/
/*
  データベースオープン時に指定できるオプション
  (以下は、SQL Server 2005 Compact Editionの場合です。それ以上のバージョンの場合は
  適時追加してください。）
*/
const DbSqlCeClient::RINIT_PROP DbSqlCeClient::m_properties[] = {
//  
	{_T("datasource")             , VT_BSTR, DBPROPOPTIONS_REQUIRED, DBPROPSET_DBINIT     , DBPROP_INIT_DATASOURCE},
	{_T("mode")                   , VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_DBINIT     , DBPROP_INIT_MODE},
	{_T("LCID")                   , VT_BSTR, DBPROPOPTIONS_OPTIONAL, DBPROPSET_DBINIT     , DBPROP_INIT_LCID},
	{_T("persist security info")  , VT_BOOL, DBPROPOPTIONS_REQUIRED, DBPROPSET_DBINIT     , DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO},
	{_T("Encrypt")                , VT_BOOL, DBPROPOPTIONS_OPTIONAL, DBPROPSET_DBINIT     , DBPROP_SSCE_ENCRYPTDATABASE},
	{_T("Password")               , VT_BSTR, DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_DBPASSWORD},
	{_T("max buffer size")        , VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_MAXBUFFERSIZE},
	{_T("max database size")      , VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_MAX_DATABASE_SIZE},
	{_T("default lock timeout")   , VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_DEFAULT_LOCK_TIMEOUT},
	{_T("default lock escalation"), VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_DEFAULT_LOCK_ESCALATION},
	{_T("flush interval")         , VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_FLUSH_INTERVAL},
	{_T("autoshrink threshold")   , VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_AUTO_SHRINK_THRESHOLD},
	{_T("temp path")              , VT_BSTR, DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_TEMPFILE_DIRECTORY},
	{_T("temp file max size")     , VT_I4  , DBPROPOPTIONS_REQUIRED, DBPROPSET_SSCE_DBINIT, DBPROP_SSCE_TEMPFILE_MAX_SIZE},
};

const int DbSqlCeClient::PROPSET_MAX = 2;


/*****************************************************************************
 * コンストラクタ
 *****************************************************************************/
DbSqlCeClient::DbSqlCeClient() :
	 m_pDBinit(0), m_pSession(0), m_pCrtCmd(0), m_pCmdtext(0), m_pTran(0),
	m_pReader(0), m_sqlceVerGUID( CLSID_SQLSERVERCE_3_0 )
{
	m_IsOpened = false;
	m_dsn[0] = 0x00;
}

DbSqlCeClient::DbSqlCeClient(const TCHAR *dsn) :
	m_pDBinit(0), m_pSession(0), m_pCrtCmd(0), m_pCmdtext(0), m_pTran(0)
{
	m_IsOpened = false;
	
	_tcsncpy( m_dsn, dsn, ARRAYSIZE(m_dsn) );
	m_dsn[ ARRAYSIZE(m_dsn) - 1] = 0x00;
}


/*****************************************************************************
 * デストラクタ
 *****************************************************************************/
DbSqlCeClient::~DbSqlCeClient()
{
	close();
}

/*****************************************************************************
 * データベースに接続します。
 *****************************************************************************/
int DbSqlCeClient::open()
{
	HRESULT hr;
	ULONG  count = 0;
	ULONG i, j;

	IDBProperties *pProp = NULL;
	// CompactのConnectionStringの指定可能なオプション数は、3が最大なので固定で確保します。
	DBPROPSET arInitPropSet[PROPSET_MAX] = {0};
	DBPROP    arProp1[4]= {0}, arProp2[10] = {0};

	//
	try{
		// インタフェースの生成
		hr = CoCreateInstance( m_sqlceVerGUID, NULL, CLSCTX_INPROC_SERVER,
				IID_IDBInitialize, (LPVOID*)&m_pDBinit);
		if( FAILED( hr ) )		throw(1);

		// プロパティの取得
		hr = m_pDBinit->QueryInterface(IID_IDBProperties, (void **)&pProp);
		if( FAILED( hr ) )		throw(2);

		// 最大数とDBPROPの配列を設定。
		arInitPropSet[0].cProperties = ARRAYSIZE(arProp1);
		arInitPropSet[0].rgProperties = arProp1;
		arInitPropSet[0].guidPropertySet = DBPROPSET_DBINIT;
		for( i = 0; i < ARRAYSIZE(arProp1); i ++ ){
			VariantInit( &(arProp1[i].vValue) );
		}

		arInitPropSet[1].cProperties = ARRAYSIZE(arProp2);
		arInitPropSet[1].rgProperties = arProp2;
		arInitPropSet[1].guidPropertySet = DBPROPSET_SSCE_DBINIT;
		for( i = 0; i < ARRAYSIZE(arProp2); i ++ ){
			VariantInit( &(arProp2[i].vValue) );
		}

		// プロパティの作成
		count = create_open_prop( arInitPropSet, ARRAYSIZE(arInitPropSet) );
		if( 0 == count ){
			throw(3);
		}
		else{
			hr = pProp->SetProperties( count, arInitPropSet );
			// プロパティのメモリ開放
			for( i = 0; i < count; i ++ ){
				for( j = 0; j < arInitPropSet[i].cProperties; j ++ ){
					if( VT_BSTR == arInitPropSet[i].rgProperties[j].vValue.vt ){
						SysFreeString( arInitPropSet[i].rgProperties[j].vValue.bstrVal );
					}
				}
			}
			pProp->Release();

			if( FAILED( hr ) )		throw(4);
		}

		// データベースに接続します。
		hr = m_pDBinit->Initialize();
        if( FAILED( hr ) ){
            throw(5);
        }

		m_IsOpened = true;

		// 次に必要なインタフェースを取得します。
		hr = m_pDBinit->QueryInterface( IID_IDBCreateSession, (void**)&m_pSession);
		if( FAILED( hr ) )		throw(6);

		//
		hr = m_pSession->CreateSession( NULL, IID_IDBCreateCommand,(IUnknown**) &m_pCrtCmd);
		if( FAILED( hr ) )		throw(7);

		//
		hr = m_pCrtCmd->CreateCommand( NULL, IID_ICommandText,(IUnknown**) &m_pCmdtext );
		if( FAILED( hr ) )		throw(8);
	}
	catch(const int err){
		if( m_IsOpened ){
			m_pDBinit->Uninitialize();
            m_IsOpened = false;
		}
		if( m_pCmdtext ){
			m_pCmdtext->Release();
			m_pCmdtext =NULL;
		}
		if( m_pCrtCmd ){
			m_pCrtCmd->Release();
			m_pCrtCmd = NULL;
		}
		if( m_pSession ){
			m_pSession->Release();
			m_pSession = NULL;
		}
		if( pProp ){
			pProp->Release();
			pProp = NULL;
		}
		if( m_pDBinit ){
			m_pDBinit->Release();
			m_pDBinit = NULL;
		}

		return err;
	}

	return 0;
}

/*****************************************************************************
 * データベースに接続します。
 *****************************************************************************/
int DbSqlCeClient::open(const TCHAR *dsn)
{
	_tcsncpy( m_dsn, dsn, ARRAYSIZE(m_dsn) );
	m_dsn[ ARRAYSIZE(m_dsn) - 1] = 0x00;

	return open();
}

/*
  データベースオープン時に指定できるオプションの調査結果
  (以下は、SQL Server 2005 Compact Editionの場合。）
*/
/*
property                 DBINIT
=============================================--
datasource				DBPROPSET_DBINIT      DBPROP_INIT_DATASOURCE
Mode					DBPROPSET_DBINIT      DBPROP_INIT_MODE
LCID					DBPROPSET_DBINIT      DBPROP_INIT_LCID
Password				DBPROPSET_SSCE_DBINIT DBPROP_SSCE_DBPASSWORD
max buffer size			DBPROPSET_SSCE_DBINIT DBPROP_SSCE_MAXBUFFERSIZE
max database size		DBPROPSET_SSCE_DBINIT DBPROP_SSCE_MAX_DATABASE_SIZE
default lock timeout	DBPROPSET_SSCE_DBINIT DBPROP_SSCE_DEFAULT_LOCK_TIMEOUT
default lock escalation	DBPROPSET_SSCE_DBINIT DBPROP_SSCE_DEFAULT_LOCK_ESCALATION
flush interval			DBPROPSET_SSCE_DBINIT DBPROP_SSCE_FLUSH_INTERVAL
autoshrink threshold	DBPROPSET_SSCE_DBINIT DBPROP_SSCE_AUTO_SHRINK_THRESHOLD
temp path				DBPROPSET_SSCE_DBINIT DBPROP_SSCE_TEMPFILE_DIRECTORY
temp file max size		DBPROPSET_SSCE_DBINIT DBPROP_SSCE_TEMPFILE_MAX_SIZE
persist security info	DBPROPSET_DBINIT ?

Encrypt	(create only)	DBPROP_SSCE_ENCRYPTDATABASE

#define DBPROP_SSCE_MAXBUFFERSIZE                   0x1FAL      // SSCE_DBINIT
#define DBPROP_SSCE_DBPASSWORD                      0x1FBL      // SSCE_DBINIT
#define DBPROP_SSCE_ENCRYPTDATABASE                 0x1FCL      // SSCE_DBINIT
#define DBPROP_SSCE_DEFAULT_LOCK_ESCALATION         0x1FDL      // SSCE_DBINIT
#define DBPROP_SSCE_TEMPFILE_DIRECTORY              0x1FEL      // SSCE_DBINIT
#define DBPROP_SSCE_DEFAULT_LOCK_TIMEOUT            0x1FFL      // SSCE_DBINIT
#define DBPROP_SSCE_AUTO_SHRINK_THRESHOLD           0x202L      // SSCE_DBINIT
#define DBPROP_SSCE_FLUSH_INTERVAL                  0x206L      // SSCE_DBINIT
#define DBPROP_SSCE_MAX_DATABASE_SIZE               0x20BL      // SSCE_DBINIT
#define DBPROP_SSCE_TEMPFILE_MAX_SIZE               0x20EL      // SSCE_DBINIT

	DBPROP_INIT_DATASOURCE	= 0x3bL,
	DBPROP_INIT_LCID	= 0xbaL,
	DBPROP_INIT_MODE	= 0x3fL,
*/
int DbSqlCeClient::create_open_prop( DBPROPSET *parPropset, int size )
{
	TCHAR key[64], value[128];
	TCHAR *pc = m_dsn, *pd = NULL;
	int propCount[ PROPSET_MAX ] = {0};
	int i = 0, index;
	DBPROP *prProp;
	int returnCode = 1;

	// ConnectionStringのパース
	while( *pc ){
		// スペースは飛ばす。
		while( _T(' ') == *pc )		pc ++;

		// 終端に来たなら終了。
		if( 0x00 == *pc )		break;

		// キーの取得
		pd = key;
		while( _T('=') != *pc ){
			*pd = *pc;
			pc ++;
			pd ++;
		}
		pc ++;
		*pd = 0x00;

		// 値の取得
		pd = value;
		while( _T(';') != *pc ){
			*pd = *pc;
			pc ++;
			pd ++;
		}
		pc ++;
		*pd = 0x00;

		// キーと値を探す
		index = -1;
		for( i = 0; i < ARRAYSIZE(DbSqlCeClient::m_properties); i ++ ){
			if( 0 == _tcsicmp(DbSqlCeClient::m_properties[i].key, key ) ){
				index = i;
				break;
			}
		}

		if( -1 != index ){
			// 種別にて判定。
			if( DBPROPSET_DBINIT == DbSqlCeClient::m_properties[index].PropSetId ){
				prProp = &(parPropset[0].rgProperties[ propCount[0] ]);
				propCount[0] ++;
			}
			else{
				prProp = &(parPropset[1].rgProperties[ propCount[1] ]);
				propCount[1] ++;

				// parPropset配列数は２個です。
				returnCode = 2;
			}

			// 見つかったので設定
			switch( DbSqlCeClient::m_properties[index].type ){
			case VT_BSTR:
				prProp->vValue.bstrVal = SysAllocString( value );
				break;
			case VT_I4:
				prProp->vValue.intVal = _ttoi(value);
				break;
			case VT_BOOL:
				if( 0 == _tcsicmp( _T("true"), value ) ){
					prProp->vValue.boolVal = VARIANT_TRUE;
				}
				else{
					prProp->vValue.boolVal = VARIANT_FALSE;
				}
				break;
			default:
				break;
			}

			prProp->colid = DB_NULLID;
			prProp->dwOptions = DbSqlCeClient::m_properties[index].reqType;
			prProp->dwPropertyID = DbSqlCeClient::m_properties[index].ulPropId;
			prProp->vValue.vt = DbSqlCeClient::m_properties[index].type;				
		}
	}

	// 実際に設定した分になるよう補正します。
	for( i = 0; i < size; i ++ ){
		parPropset[i].cProperties = propCount[i];
	}

	return returnCode;
}


/*****************************************************************************
 * データベースへ接続中が調べる
 *****************************************************************************/
bool DbSqlCeClient::is_open()
{
	return m_IsOpened;
}

/*****************************************************************************
 * データベースとの接続を閉じます。
 *****************************************************************************/
void DbSqlCeClient::close()
{
	if( m_pTran ){
		rollback_tran();
		// 中でNULL入れしてます。
	}

	if( m_IsOpened ){
		m_pDBinit->Uninitialize();
        m_IsOpened = false;
	}

	if( m_pCmdtext ){
		m_pCmdtext->Release();
		m_pCmdtext = NULL;
	}
	
	if( m_pCrtCmd ){
		m_pCrtCmd->Release();
		m_pCrtCmd = NULL;
	}

	if( m_pSession ){
		m_pSession->Release();
		m_pSession = NULL;
	}

	if( m_pDBinit ){
		m_pDBinit->Release();
		m_pDBinit = NULL;
	}
}


/*****************************************************************************
 * DELETE、UPDATE、INSERT系のクエリを実行します。
 *****************************************************************************/
LONG DbSqlCeClient::execute(const TCHAR *query)
{
	HRESULT hr;
	LONG lRows = 0;

	try{
		// クエリ設定
		hr = m_pCmdtext->SetCommandText( DBGUID_DBSQL, query );
		if( FAILED( hr ) )		throw(-1);

		// クエリ実行
		hr = m_pCmdtext->Execute( NULL, IID_NULL, NULL, &lRows, NULL );
		if( FAILED( hr ) )		throw(-2);

	}
	catch( const int err){
		return err;
	}
	
	return lRows;
}

/*****************************************************************************
 * SELECT COUNTなどの数値１つが返るクエリを実行します。
 *****************************************************************************/
LONG DbSqlCeClient::execute_scaler(const TCHAR *query)
{
	DbSqlCeDataReader *pReader;
	DbDataReader::SEEK_RESULT result;
	int ret;
	LONG count = 0;
	
	// クエリを実行します。
	pReader = exetute_select( query );
	if( NULL == pReader ){
		return -1;
	}

	// 
	result = pReader->read();
	if( result != DbDataReader::SEEK_OK ){
		this->close_datareader( pReader );

		return -1;
	}

	// 結果を取り出します。
	ret = pReader->get_column( 0, &count, sizeof(LONG), DBTYPE_I4 );
	if( -1 == ret ){
		this->close_datareader( pReader );

		return -1;
	}

	this->close_datareader( pReader );

	return count;
}

/*****************************************************************************
 * SELECT系のクエリを実行します。
 * SELECTの結果はDbSqlCeDataReaderクラスを実行して取得してください。
 *****************************************************************************/
DbSqlCeDataReader* DbSqlCeClient::exetute_select(const TCHAR *query)
{
	HRESULT hr;
	LONG lRows = 0;
	IRowsetPosition *pRowsetPos = NULL;
	ICommandProperties *pICmdProps = NULL;
	DBPROPSET dbPropSet[1] = {0};
	DBPROP    dbProps[1] = {0};

	try{
		// クエリ設定
		hr = m_pCmdtext->SetCommandText( DBGUID_DBSQL, query );
		if( FAILED( hr ) )		throw(-1);

		hr = m_pCmdtext->QueryInterface(IID_ICommandProperties, (void**) &pICmdProps);
		if( FAILED( hr ) )		throw(-1);

		dbProps[0].dwPropertyID   = DBPROP_CANFETCHBACKWARDS;
		dbProps[0].dwOptions      = DBPROPOPTIONS_REQUIRED;
		dbProps[0].vValue.vt      = VT_BOOL;
		dbProps[0].vValue.boolVal = VARIANT_TRUE;

		dbPropSet[0].cProperties = 1;
		dbPropSet[0].rgProperties = dbProps;
		dbPropSet[0].guidPropertySet = DBPROPSET_ROWSET;

		hr = pICmdProps->SetProperties(ARRAYSIZE(dbPropSet), dbPropSet);
		if( FAILED( hr ) ){
			throw(-1);
		}
		else{
			pICmdProps->Release();
			pICmdProps = NULL;
		}

		// クエリ実行
		hr = m_pCmdtext->Execute( NULL, IID_IRowsetPosition, NULL, NULL, (IUnknown**)&pRowsetPos );
		if( FAILED( hr ) )		throw(-2);


		// リーダー作成。
		m_pReader = new DbSqlCeDataReader( pRowsetPos );
		if( NULL == m_pReader )	throw(-3);

		// 初期化が正常終了したか確認します。
		if( false == m_pReader->m_IsInit ){
			throw(-4);
		}
	}
	catch(...){
        if( m_pReader ){
            delete m_pReader;
            m_pReader = NULL;
        }
		if( pICmdProps )	pICmdProps->Release();
		if( pRowsetPos )	pRowsetPos->Release();

		return NULL;
	}
	

	return m_pReader;
}

/*****************************************************************************
 * DbSqlCeDataReaderインスタンスを破棄します。
 *****************************************************************************/
void DbSqlCeClient::close_datareader( DbSqlCeDataReader *reader )
{
	if( reader ){
		delete reader;
	}
}

/*****************************************************************************
 * トランザクションを開始します。（抽象クラスとの整合のためにあります。）
 *****************************************************************************/
int DbSqlCeClient::begin_tran( int level )
{
	return begin_tran( (ISOLEVEL)level );
}

/*****************************************************************************
 * トランザクションを開始します。
 *****************************************************************************/
int DbSqlCeClient::begin_tran( ISOLEVEL level )
{
	HRESULT hr;

	if( m_pTran ){
		return 0;
	}

	hr = m_pSession->QueryInterface( IID_ITransactionLocal, (void**)m_pTran );
	if( FAILED( hr ) ){
		return -1;
	}

	hr = m_pTran->StartTransaction( level, 0, NULL, NULL );
	if( FAILED( hr ) ){
		m_pTran->Release();
		m_pTran = NULL;
		return -1;
	}	

	return 0;
}

/*****************************************************************************
 * トランザクションをコミットします。
 *****************************************************************************/
int DbSqlCeClient::commit_tran()
{
	HRESULT hr;
	int result = -1;

	if( NULL == m_pTran ){
		return -1;
	}

	hr = m_pTran->Commit( FALSE, XACTTC_SYNC, 0 );
	if( FAILED( hr ) ){
		// キャンセルします。
		m_pTran->Abort( NULL, FALSE, FALSE ); 

		m_pTran->Release();
		m_pTran = NULL;

		result =  -1;
	}

	return result;
}

/*****************************************************************************
 * トランザクションをロールバックします。
 *****************************************************************************/
int DbSqlCeClient::rollback_tran()
{
	HRESULT hr;
	int result = -1;

	if( NULL == m_pTran ){
		return -1;
	}

	hr = m_pTran->Abort( NULL, FALSE, FALSE );
	if( FAILED( hr ) ){
		m_pTran->Release();
		m_pTran = NULL;

		result =  -1;
	}

	return result;
}
#endif
