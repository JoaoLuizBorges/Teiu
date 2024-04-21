package com.joaob.backendsume.domain.scans;

import com.joaob.backendsume.controller.ScansController;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.scheduling.support.SimpleTriggerContext;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

public interface ScanRepository extends JpaRepository<Scans, Long> {

    Page<Scans> findBySerial(Pageable paginacao);

    @Transactional
    void deleteBySerial(String serial);
}
