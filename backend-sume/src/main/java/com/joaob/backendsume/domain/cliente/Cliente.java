package com.joaob.backendsume.domain.cliente;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;

@Table(name = "cliente")
@Entity(name = "Cliente")
@Getter
@NoArgsConstructor
@AllArgsConstructor
@EqualsAndHashCode(of = "id")
public class Cliente {

    private Long id;
    private String cpf;
    private String serial;
    private boolean ativo;

    public Cliente(DadosCadastroCliente dados) {
        this.ativo = true;
        this.cpf = dados.cpf();
        this.serial = dados.serial();
    }

    public void atualizarInformacoes(DadosAtualizacaoCliente dados) {
        if (dados.cpf() != null) {
            this.cpf = dados.cpf();
        }
        if (dados.serial() != null) {
            this.serial = dados.serial();
        }
    }

    public void excluir() {
        this.ativo = false;
    }
}
