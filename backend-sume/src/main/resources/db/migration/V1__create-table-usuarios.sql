create table usuarios(
    id bigint not null auto_increment,
    nome varchar(255) not null,
    cpf varchar(100) not null,
    login varchar(100) not null,
    senha varchar(255) not null,

    primary key(id)

);
