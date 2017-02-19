<!-- box main start -->
<div class="boxmain">
<!-- -->
<?
	/* 
		ソースリストを取得
	*/
	$srclist = array();
	$target_ext = array( '.cpp', '.h' );
	
	$dir = opendir("./");
	while($file = readdir($dir)) {
		if(is_file("./$file")) {
			foreach( $target_ext as $ext ){
				if( stristr( $file, $ext )) {
					$srclist[] = $file;
					break;
				}
			}
        }
	}
	closedir($dir);	
	
	/* 所定のファイルを出力 */
	foreach( $srclist as $filename ){
		echo '<h2>' . $filename . '</h2>' . "\n";
		echo '<pre class="source">'. "\n";
			// ファイルの中身を取得
			$fileres = file_get_contents( "./$filename" );
            //$fileres = str_replace( "\n", "<br>", $fileres );
			echo mb_convert_encoding( $fileres, "UTF-8", "SJIS");
		echo '</pre>'. "\n";
	}
?>
</div>
<!-- box main end -->
