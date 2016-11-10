<?php

error_reporting(0);
@ini_set('display_errors', 0);

/**
* Класс определяет строчку прайса
*/
class Price
{
	function Price($aa)
	{
		foreach ($aa as $k=>$v)
            $this->$k = $aa[$k];
	}
}

function parseString($values)
{
    for ($i=0; $i < count($values); $i++) {
        $str[$values[$i]["tag"]] = $values[$i]["value"];
    }
    return new Price($str);
}


function parse_file()
{
	if (file_exists('japancar_krasnoyarsk.xml')) {
		$data = implode("", file('japancar_krasnoyarsk.xml'));
		$parser = xml_parser_create();
		xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
		xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);
		xml_parse_into_struct($parser, $data, $values, $tags);
		xml_parser_free($parser);
		// проход через структуры
		foreach ($tags as $key=>$val) {
			if ($key == "data") {
				$molranges = $val;
				// каждая смежная пара значений массивов является верхней и
				// нижней границей определения
				for ($i=0; $i < count($molranges); $i+=2) {
					$offset = $molranges[$i] + 1;
					$len = $molranges[$i + 1] - $offset;
					$tdb[] = parseString(array_slice($values, $offset, $len));
				}
			} else {
				continue;
			}
		}
		return $tdb;

	} else {
		exit('Не удалось открыть файл japancar_krasnoyarsk.xml.');
	}
}

$result = parse_file();

$file = fopen('price.csv', 'w');
$foto = fopen('fotolist.txt', 'w');
foreach ($result as $line) {
	fputcsv($file, (array) $line);
	fwrite($foto, $line->photo_name . "\n");
}
fclose($file);
fclose($foto);