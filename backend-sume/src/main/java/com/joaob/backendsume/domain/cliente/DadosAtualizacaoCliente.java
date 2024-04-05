package com.joaob.backendsume.domain.cliente;

import com.joaob.backendsume.domain.endereco.DadosEndereco;
import jakarta.validation.constraints.NotNull;

public record DadosAtualizacaoCliente(

        @NotNull
        Long id,
        @NotNull
        String cpf,

        @NotNull
        String serial,

        DadosEndereco endereco) {
}
