<?php
namespace src\core;

use src\core\bd\BD;

class Peticion
{

    static $formatoRespuesta = 0;

    static $encapsulado = true;

    const WEB = 0;

    const JSON = 1;

    const XML = 2;

    static function coger()
    {
        if (strpos($_SERVER["HTTP_ACCEPT"], "application/json") !== false) {
            self::$formatoRespuesta = self::JSON;
            header("Content-Type: application/json; utf-8");
        } elseif (strpos($_SERVER["HTTP_ACCEPT"], "text/html") !== false) {
            self::$formatoRespuesta = self::WEB;
        } elseif (strpos($_SERVER["HTTP_ACCEPT"], "application/xml") !== false) {
            self::$formatoRespuesta = self::XML;
            header("Content-Type: application/xml; utf-8");
        } else {
            self::$formatoRespuesta = self::WEB;
        }

        
        //{"a":"1","b":[{"c":2},{"c":3}]} en JSON
        //["a"=>1,"b"=>["c"=>2,"c"=>3]] en PHP
        //$respuesta=["a"=>1,"b"=>[["c"=>2],["c"=>3]]];
        $respuesta=["clientas"=>[["nombre"=>"Raúl", "id"=>1],["nombre"=>"Juan", "id"=>2]]];
        $res=BD::ejecutar("select * from unatabla;");
        if($res){
            $respuesta=["tablas"=>BD::filas($res)];
        }
        return ["respuesta"=>$respuesta];
        // print_r($_SERVER);
        // echo "<br/>";
    }
}