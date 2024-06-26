package com.joaob.backendsume.domain.cliente;

import com.joaob.backendsume.domain.endereco.DadosEndereco;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;

public record DadosCadastroCliente(

        @NotBlank
        String usuario,

        @NotBlank
        @Pattern(regexp = "^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*-]).{8,}$")
        String senha,

        @NotBlank
        String ident,
        @NotBlank
        String cpf_cnpj,

        @NotBlank
        String serial,

        @NotNull @Valid DadosEndereco endereco) {
}
