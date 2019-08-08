<?php 
namespace src\core\bd;
abstract  class BD{
    static protected $instancias=[];
    static function conexion($cadenaConexion){
        if(key($cadenaConexion,self::$instancias) && self::$instancias[$cadenaConexion]!==null){
            return self::$instancias[$cadenaConexion];
        }
        $datos=self::cogerDatosConexion($cadenaConexion);
        switch ($datos["tipo"]){
            case "mysql":
                self::$instancias[$cadenaConexion]=new BDMY($datos);
                break;
        }
        
        return self::$instancias[$cadenaConexion];
    }
    static function ejecutar($consulta, $opciones=[]){
        //TODO: Cambiar la conexión para que no dependa exclusivamente de API
        $con=self::conexion(API);
        return ["conexion"=>$con,"res"=>$con->_ejecutar($consulta, $opciones)];
    }
    static function filas($res){
        return $res["conexion"]->_filas($res["res"]);
    }
    static function cogerDatosConexion($cadenaConexion){
        $partes=explode(":", $cadenaConexion, 2);
        
        $partes2=explode("@", $partes[1], 2);
        $usuario=explode("\\", $partes2[0], 2);
        $conexion=explode("\\", $partes2[1], 2);
        $servidor=explode(":", $conexion[0], 2);
        return ["tipo"=>$partes[0],"usuario"=>$usuario[0],"clave"=>$usuario[1],"servidor"=>$servidor[0],"puerto"=>$servidor[1],"esquema"=>$conexion[1]];
    }
}