<?php
namespace src\core;

use src\core\bd\BD;
use src\core\bd\BDMY;

class Peticion
{

    static $formatoRespuesta = 0;

    static $encapsulado = true;

    const WEB = 0;

    const JSON = 1;

    const XML = 2;

    static function coger(){
       if (strpos($_SERVER["HTTP_ACCEPT"], "application/json") !== false) {
         self::$formatoRespuesta = self::JSON;
         header ( "Content-Type: application/json; utf-8" );
      } elseif (strpos ( $_SERVER ["HTTP_ACCEPT"], "text/html" ) !== false) {
         self::$formatoRespuesta = self::WEB;
      } elseif (strpos ( $_SERVER ["HTTP_ACCEPT"], "application/xml" ) !== false) {
         self::$formatoRespuesta = self::XML;
         header ( "Content-Type: application/xml; utf-8" );
      } else {
         self::$formatoRespuesta = self::WEB;
     }
     $datosPeticion=explode("?",$_SERVER["REQUEST_URI"],2);
     $ruta=explode('/',$datosPeticion[0]);
     $opciones=[];//["conexion"=>"mysql:bot\\000@localhost:3306\\chat_bot"]
     if(count($ruta)>1){
        $tablas=BD::tablas($opciones);//["conexion"=>"mysql:bot\\000@localhost:3306\\chat_bot"]);
        if(in_array($ruta[1], $tablas)){
           if(count($ruta)>2 && trim($ruta[2])!==""){
              $opciones["id"]=explode(":",$ruta[2]);
              $respuesta=BD::listar($ruta[1],$opciones);
              return ["respuesta"=>$respuesta];
           }else{
              $respuesta=BD::listar($ruta[1],$opciones);
              return ["respuesta"=>[$ruta[1]=>$respuesta]];
           }
        }else{
           //header("HTTP/1.1 400 OK");//
        }
        
        
     }
     return ["respuesta"=>["Server"=>print_r($_SERVER,true)]];
        
        
        //{"a":"1","b":[{"c":2},{"c":3}]} en JSON
        //["a"=>1,"b"=>["c"=>2,"c"=>3]] en PHP
        //$respuesta=["a"=>1,"b"=>[["c"=>2],["c"=>3]]];
        /*$respuesta=["clientas"=>[["nombre"=>"Raúl", "id"=>1],["nombre"=>"Juan", "id"=>2]]];
        $res=BD::ejecutar("select * from unatabla;");
        if($res){
            $respuesta=["tablas"=>BD::filas($res)];
        }*/
        //$respuesta=BD::cogerEstructura("unatabla");
        //$conexion=BD::conexion("mysql:bot\\000@localhost:3306\\chat_bot");
        /*
         * [
            "servidor"=>"localhost",
            "puerto"=>3303,
            "usuario"=>"bot",
            "clave"=>"000",
            "esquema"=>"chat-bot"]
         */
                
        $tabla=$_REQUEST["tabla"];
        $respuesta=BD::listar($tabla,["conexion"=>"mysql:bot\\000@localhost:3306\\chat_bot"]);
        return ["respuesta"=>[$tabla=>$respuesta]];
        // print_r($_SERVER);
        // echo "<br/>";
    }
}