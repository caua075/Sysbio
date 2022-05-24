    <?php
date_default_timezone_set('America/Sao_Paulo');
$data = date("d/m/Y");
$hora = date("H:i:s");
//-----------------------------------------------  
$servidor = "localhost";
$porta = '****';
$bancoDeDados = "projetosysbio"; //Nome do Banco
$usuario = "postgres"; //Usuário do Pgadmin
$senha = "2321"; //Senha do PGadmin
$conexao = pg_connect("host=$servidor port=$porta dbname=$bancoDeDados user=$usuario password=$senha");
//------------------------------------------------
$cod_ = $_GET['cod'];

    if(!$conexao) {
        die("Não foi possível se conectar ao banco de dados.");
    }
    else{
    //SELECT PARA ACHAR O CÓDIGO DO CARTÃOE O NOME
    $sql = "SELECT * FROM pessoa WHERE carrfid = '$cod_'";
    $result = pg_query($conexao, $sql);
    $consulta = pg_fetch_assoc($result);
    $nome = $consulta['nome'];
    $pront = $consulta['pront'];
    
    if($nome == ""){
        echo 0;
    }
    else{
        echo 1;
            $selecthora = "select e.data, e.horaE, e.horaS from entradaSaida e where e.pront = '$pront' and e.data = TO_CHAR(NOW(), 'dd/mm/YYYY') ORDER BY e.horaE DESC;";
            $result2 = pg_query($conexao, $selecthora);
            $consulta = pg_fetch_assoc($result2);
            $he = $consulta['horae'];
            $hs = $consulta['horas'];
            if ($he == null || ($he != null && $hs != null)) {
                $s = "insert into entradaSaida(data, horaE, pront) values('$data','$hora','$pront');";
                $result = pg_query($conexao, $s);
            }
            else{
                $v = "update entradaSaida set horaS = '$hora' where pront = '$pront' AND horaE = '$he'" ;
                $result = pg_query($conexao, $v);
            }
        }
    }
    pg_close($conexao);
        ?>
