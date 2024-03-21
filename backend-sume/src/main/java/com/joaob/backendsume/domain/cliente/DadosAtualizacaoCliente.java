package com.joaob.backendsume.domain.cliente;

import jakarta.validation.constraints.NotNull;

public record DadosAtualizacaoCliente(

        @NotNull
        Long id,
        @NotNull
        String cpf,

        @NotNull
        String serial
) {
}
