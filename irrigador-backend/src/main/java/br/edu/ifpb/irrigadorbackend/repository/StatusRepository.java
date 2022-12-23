package br.edu.ifpb.irrigadorbackend.repository;

import br.edu.ifpb.irrigadorbackend.entity.Status;
import org.springframework.data.jpa.repository.JpaRepository;

public interface StatusRepository extends JpaRepository<Status, Long> {
}
