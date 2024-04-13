package com.joaob.backendsume.domain.cliente;

public record DadosDetalhamentoCliente(Long id, String ident, String cpf_cnpj, String serial) {

    public DadosDetalhamentoCliente(Cliente cliente) {
        this(cliente.getId(), cliente.getIdent(), cliente.getCpf_cnpj(), cliente.getSerial());
    }
}
