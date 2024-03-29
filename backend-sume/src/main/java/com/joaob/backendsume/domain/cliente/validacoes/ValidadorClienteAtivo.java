package com.joaob.backendsume.domain.cliente.validacoes;

import com.joaob.backendsume.domain.ValidacaoException;
import com.joaob.backendsume.domain.cliente.ClienteRepository;
import com.joaob.backendsume.domain.cliente.DadosDetalhamentoCliente;
import org.springframework.beans.factory.annotation.Autowired;

public class ValidadorClienteAtivo {

    @Autowired
    private ClienteRepository repository;

    public void validar(DadosDetalhamentoCliente dados) {
        var clienteAtivo = repository.findAtivoByCpf(dados.cpf());
        if(!clienteAtivo) {
            throw new ValidacaoException("Usuário não é ativo");
        }
    }
}
