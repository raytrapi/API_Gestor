<?php
require_once __DIR__ . "/log.inc";
use src\seguridad\Log;
ini_set("display_errors", "Off");

 error_reporting(E_ALL | E_RECOVERABLE_ERROR);
function gestionErrores($num_err, $mensaje_err, $nombre_fichero, $num_linea, $vars)
{
    Log::error("[" . $nombre_fichero . "(" . $num_linea . ")]  " . $mensaje_err);
    return;
}

 set_error_handler("gestionErrores");
function gestionFatal()
{
    $error = error_get_last();
    if ($error !== NULL) {

        $errno = $error["type"];
        $errfile = $error["file"];
        $errline = $error["line"];
        $errstr = $error["message"];
        Log::fatal($errstr);
        return false;
    }

    return;
}
register_shutdown_function("gestionFatal");