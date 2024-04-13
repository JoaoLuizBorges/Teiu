package com.joaob.backendsume.domain.cliente;

import com.joaob.backendsume.domain.endereco.Endereco;
import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import org.hibernate.validator.constraints.br.CNPJ;
import org.hibernate.validator.constraints.br.CPF;

@Table(name = "cliente")
@Entity(name = "Cliente")
@Getter
@NoArgsConstructor
@AllArgsConstructor
@EqualsAndHashCode(of = "id")
public class Cliente {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @CNPJ
    @CPF
    private String cpf_cnpj;
    private String ident;
    private String serial;
    private Boolean ativo;
    private String usuario;
    private String senha;

    @Embedded
    private Endereco endereco;

    public Cliente(DadosCadastroCliente dados) {
        this.usuario = dados.usuario();
        this.senha = dados.senha();
        this.ativo = true;
        this.ident = dados.ident();
        this.cpf_cnpj = dados.cpf_cnpj();
        this.serial = dados.serial();
        this.endereco = new Endereco(dados.endereco());
    }

    public void atualizarInformacoes(DadosAtualizacaoCliente dados) {
        if (dados.cpf_cnpj() != null) {
            this.cpf_cnpj = dados.cpf_cnpj();
        }
        if (dados.serial() != null) {
            this.serial = dados.serial();
        }
        if (dados.endereco() != null) {
            this.endereco.atualizarEndereco(dados.endereco());
        }
    }
        public void excluir() {
            this.ativo = false;
        }
}
