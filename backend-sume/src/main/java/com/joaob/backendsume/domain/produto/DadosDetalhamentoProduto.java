package com.joaob.backendsume.domain.produto;

public record DadosDetalhamentoProduto(Long id, String serial, String modelo) {

    public DadosDetalhamentoProduto(Produto produto) {
        this(produto.getId(), produto.getSerial(), produto.getModelo());
    }
}
