package com.joaob.backendsume.domain.produto;

import jakarta.validation.constraints.NotBlank;

public record DadosCadastroProduto(
        @NotBlank
        String serial,

        @NotBlank
        String modelo) {
}
