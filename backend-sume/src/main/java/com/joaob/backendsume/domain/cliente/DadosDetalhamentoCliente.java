package com.joaob.backendsume.domain.cliente;

public record DadosDetalhamentoCliente(Long id, String cpf, String serial) {

    public DadosDetalhamentoCliente(Cliente cliente) {
        this(cliente.getId(), cliente.getCpf(), cliente.getSerial());
    }
}
