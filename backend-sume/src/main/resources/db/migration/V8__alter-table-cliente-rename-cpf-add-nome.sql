ALTER TABLE cliente
    RENAME COLUMN cpf TO cpf_cnpj,
    ADD COLUMN ident varchar(255) not null;