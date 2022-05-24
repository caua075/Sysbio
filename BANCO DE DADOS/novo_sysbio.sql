CREATE TABLE pessoa(
    pront character varying(15) NOT NULL,
    nome character varying(50),
    sexo character varying(15),
    cpf character varying(20),
    telefone character varying(15),
    cidade character varying(30),
    rua character varying(50),
    numero character varying(10),
    cep character varying(20),
    tipo character varying(20),
    carRFID character varying(30),
	CONSTRAINT pessoa_pkey PRIMARY KEY (pront));

SELECT * FROM pessoa;
-----------------------------------------------------------------------------------
CREATE TABLE administrador (
    login character varying(50),
    senha character varying(16),
    pront character varying(15),
	CONSTRAINT administrador_pkey PRIMARY KEY (pront),
	CONSTRAINT administrador_pront_fkey FOREIGN KEY (pront) REFERENCES pessoa(pront));
	
SELECT * FROM administrador;
-----------------------------------------------------------------------------------
CREATE TABLE entradasaida(
    data character varying(10),
    horaE character varying(10),
    horaS character varying(10),
    pront character varying(15),
    CONSTRAINT entradasaida_pront_fkey FOREIGN KEY (pront) REFERENCES pessoa(pront));

SELECT * FROM entradasaida;