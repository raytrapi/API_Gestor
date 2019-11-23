<?php 
namespace src\core\bd;
require_once "bd.inc";
use src\seguridad\Log;

class BDMY extends BD{
    private $mysqli;
    public function __construct($datos){
        //gestorBD:usuario\password@host:puerto\esquema
        //mysql:api\000@localhost:3306\api
        
//        $datos=self::cogerDatosConexion($cadenaConexion);
        
        $this->mysqli = new \mysqli("p:".$datos["servidor"].":".$datos["puerto"], $datos["usuario"], $datos["clave"], $datos["esquema"]);
        $this->mysqli->set_charset("utf8");
        return $this;
    }
    public function _ejecutar($consulta, $opciones = []){
        $sentencia = $this->mysqli->prepare($consulta);
        if($sentencia->execute()){
            return $sentencia->get_result();
        }else{
            return null;
        }
        
    }
    public function _filas($res){
        return mysqli_fetch_all($res,MYSQLI_ASSOC);
    }
    
    public function _cogerEstructura($nombreTabla){
        $respuesta=[];
        $res=$this->_ejecutar("desc ".$nombreTabla);
        if($res){
           $filas=$this->_filas($res);
           foreach ($filas as $fila){
              $campo=[];
              $tipo=$fila["Type"];
              $partes=explode('(',$tipo,2);
              
              $campo["tipo"]=$partes[0];
              if(count($partes)>1){
                 $campo["longitud"]=substr($partes[1],0,strlen($partes[1])-1);
              }
              $campo["nulo"]=$fila["Null"];
              $campo["clave"]=$fila["Key"]=="PRI";
              $campo["defecto"]=$fila["Default"];
              $respuesta[$fila["Field"]]=$campo;
           }
           return ["tabla"=>["nombre"=>$nombreTabla],"campos"=>$respuesta];
        }
        return null;
    }
}
