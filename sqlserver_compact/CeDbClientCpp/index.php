<?
require_once( $_SERVER['DOCUMENT_ROOT'] . '/definition.php' );
require_once( LIB_DIR . 'classfile.php' );


$maker = new CFormMaker;

$maker->WriteDiv_header( false );
$maker->WriteDiv_SubNavi( 'software' );

// メインの出力
$maker->WriteDiv_main( CONTENT_SOFTWARE . 'src/CeDbClientCpp/main_index.php' );

$maker->WriteDiv_footer();
?>
