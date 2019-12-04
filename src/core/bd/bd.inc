<?php 
namespace src\core\bd;
require_once __DIR__."\..\..\..\conf\conexion.inc";
abstract  class BD{
    static protected $instancias=[];
 
    static function conexion($cadenaConexion){
        if(key_exists($cadenaConexion,self::$instancias) && self::$instancias[$cadenaConexion]!==null){
            return self::$instancias[$cadenaConexion];
        }
        $datos=self::cogerDatosConexion($cadenaConexion);
        switch ($datos["tipo"]){
            case "mysql":
                if(!class_exists("src\core\bd\BDMY", false)){
                    require_once __DIR__.'\bdmy.inc';
                }
                self::$instancias[$cadenaConexion]=new BDMY($datos);
                break;
        }
        
        return self::$instancias[$cadenaConexion];
    }
    /**
     * Ejectua una consulta
     * @param string $consulta consulta, los valores con parámetros se definen con ?
     * @param array $opciones ["conexion"=>string,]
     * @return mixed["conexion"=>conexion, "res"=>resultado_ejecucion]
     */
    static function ejecutar($consulta, $opciones=[]){
       $con=self::cogerConexion($opciones);
       return ["conexion"=>$con,"res"=>$con->_ejecutar($consulta, $opciones)];
    }
    /**
     * Devuelve un array de valores obtenidos de una select
     * @param resource $res devuelto por el método ejecutar
     * @param boolean $asociativo por defecto es true
     * @return mixed
     */
    static function filas($res, $asociativo=true){
        //TODO: Comprobar que todo va bien
       return $res["conexion"]->_filas($res["res"],$asociativo);
    }
    static function cogerDatosConexion($cadenaConexion){
        $partes=explode(":", $cadenaConexion, 2);
        
        $partes2=explode("@", $partes[1], 2);
        $usuario=explode("\\", $partes2[0], 2);
        $conexion=explode("\\", $partes2[1], 2);
        $servidor=explode(":", $conexion[0], 2);
        return ["tipo"=>$partes[0],"usuario"=>$usuario[0],"clave"=>$usuario[1],"servidor"=>$servidor[0],"puerto"=>$servidor[1],"esquema"=>$conexion[1]];
    }
    static function cogerConexion($opciones=[]){
       $CON=CONEXION_DEFECTO;
       if(key_exists("conexion", $opciones)){
          $CON=$opciones["conexion"];
       }
       return self::conexion($CON);
    }
    const TIPO_VARCHAR=0;
    const TIPO_NUMBER=1;
    const TIPO_DATETIME=2;
    /**
     * Devuelve la estructura de una tabla
     * @param string $nombreTabla el nombre de la tabla
     * @param mixed $opciones [conexion=>conexion,]
     * @return mixed  ["tabla"=>["nombre"=>string,"esquema"=>string],"campos"=>["campo"=>["tipo"=>TIPO_, "longitud"=>numero]]]
     */
    static function cogerEstructura($nombreTabla, $opciones=[]){
      return self::cogerConexion($opciones)->_cogerEstructura($nombreTabla);
    }
    
    /**
     * Devolvemos los datos de la consulta
     * @param string $tabla
     * @param array $opciones
     * @return array con filas
     */
   static function listar($tabla, $opciones = []) {
      $select = self::select ( $tabla, $opciones );
      $opciones["parametros"]=$select["parametros"];
      $res = self::ejecutar ($select["consulta"] , $opciones );
      if ($res) {
         if (key_exists ( "id", $opciones )) {
            $filas = self::filas ( $res );
            if (count ( $filas ) != 1) {
               return null;
            } else {
               return $filas [0];
            }
         } else {
            return self::filas ( $res );
         }
      }
      return [ ];
   }
    
    /**
     * Genera la select para su ejecución
     * @param string $tabla Nombre de la tabla
     * @param mixed $opciones ["join"=>boolean,"limite"=>int]
     * @return string
     */
    static function select($tabla,$opciones=[]){
        $estructura=self::cogerEstructura($tabla,$opciones);
        $consulta="select ";
        $i=0;
        $claves=[];
        foreach ($estructura["campos"] as $nombre=>$campo){
            $consulta.=(($i++>0)?", ":"").$nombre;
            if($campo["clave"]){
               $claves[]=$nombre;
            }
        }
        $consulta.=" from ".$estructura["tabla"]["nombre"];
        
        //Where
        $where="";
        $parametros=[];
        if(key_exists("id", $opciones)){
           $numeroClaves=count($opciones["id"]);
           for($i=0;$i<$numeroClaves;$i++){
              $where.=($where==""?" where ":" and ").$claves[$i]."=?";
              $parametros[$claves[$i]]=["tipo"=>"i","valor"=>$opciones["id"][$i]];
           }
        }
        if($where!==""){
           $consulta.=$where;
        }
        return ["consulta"=>$consulta,"parametros"=>$parametros];
    }
    
    static function tablas($opciones=[]){
       return self::cogerConexion($opciones)->_tablas();
    }
}