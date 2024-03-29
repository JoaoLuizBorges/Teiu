ALTER TABLE cliente
ADD logradouro varchar(100) not null,
ADD bairro varchar(100) not null,
ADD cep varchar(9) not null,
ADD complemento varchar(100),
ADD numero varchar(20),
ADD uf char(2) not null,
ADD cidade varchar(100) not null;