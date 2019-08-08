<?php
namespace src\seguridad;

class Log
{

    static function debug($msg)
    {
        self::grabar("debug.log", $msg);
    }

    static function error($msg)
    {
        self::grabar("error.log", $msg);
    }

    static function fatol($msg)
    {
        self::grabar("error.log", $msg);
    }

    static function sql($msg)
    {
        self::grabar("sql.log", $msg);
    }

    static private function grabar($fichero, $msg)
    {
        $fecha = date("d-m-Y H:i:s");

        $path = LOG_PATH . $fichero;
        $fo = fopen($path, "a");
        if ($fo) {
            fwrite($fo, $fecha . "\t" . $msg . "\r\n");
            fclose($fo);
        }
    }
}