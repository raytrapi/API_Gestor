<?php

use src\seguridad\Log;
use src\core\Peticion;
use src\core\Mensaje;
use src\core\bd\BDMY;


require 'inicializacion.inc';
require_once 'src\core\peticion.inc';
ob_start();




$info=Peticion::coger();
Log::debug("Termino");
$contenidos=ob_get_contents();
ob_end_clean();

switch (Peticion::$formatoRespuesta){
    case Peticion::WEB:
        ?>
<!DOCTYPE html>
<html>
	<head>
		<title>Mi primer PHP</title>
	</head>
	<body>
		<h1>Resultado:</h1>
		<?=$contenidos?>
	</body>
</html>
<?php 
        break;
    case Peticion::JSON:
        echo Mensaje::mostrar($info["respuesta"]);
        //echo "{\"respuesta\":\"SOY JSON\"}";
        break;
    case Peticion::XML:
        echo "<respueta>SOY XML</respuesta>";
        break;
}

