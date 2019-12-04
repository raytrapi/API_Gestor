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
        $parametros=[];
        if(key_exists("parametros", $opciones)){
           $parametros=$opciones["parametros"];
           
           
        }
        if(count($parametros)){
           $tipos="";
           $valores=[];
           foreach ($parametros as $parametro){
              $tipos.=$parametro["tipo"];
              $valores[]=$parametro["valor"];
           }
           $param=array_merge([$tipos], $valores);
           
           call_user_func_array([$sentencia,"bind_param"], array_merge([$tipos], $valores));
           //$sentencia->bind_param($tipos, ...$valores);
        }
        if($sentencia->execute()){
            return $sentencia->get_result();
        }else{
            return null;
        }
        
    }
    
    public function _filas($res, $asociativo=true){
        return mysqli_fetch_all($res,$asociativo?MYSQLI_ASSOC:MYSQLI_NUM);
    }
    
    public function _cogerEstructura($nombreTabla){
        $respuesta=[];
        $res=$this->_ejecutar("desc ".$nombreTabla);
        if($res){
           $filas=$this->_filas($res);
           foreach ($filas as $fila){
              $campo=[];
              //TODO Modifcar el tipo de dato para que sea uno de los denifinidos en las constantes
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
    public function _tablas(){
       $tablas=$this->_filas($this->_ejecutar("show tables"),false);
       $resultado=[];
       foreach ($tablas as $tabla){
          $resultado[]=$tabla[0];
       }
       return $resultado;
    }
}
