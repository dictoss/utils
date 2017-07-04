;;;;;;;;;;;;;;;;;;;;;;
;; gdb
;;;;;;;;;;;;;;;;;;;;;
(setq gdb-many-windows nil)
(setq gdb-use-separate-io-buffer t)

;;;;;;;;;;;;;;;;;;;;;;;
;; C
;;;;;;;;;;;;;;;;;;;;;
(defun my-c-mode-hook ()
;  (c-set-style "linux")
  (c-set-style "bsd")
  (setq tab-width 4)
  (setq c-basic-offset tab-width))
(add-hook 'c-mode-hook 'my-c-mode-hook)


;;;;;;;;;;;;;;;;;;;;;;
;; C++
;;;;;;;;;;;;;;;;;;;;;
(defun my-c++-mode-hook ()
;  (c-set-style "linux")
  (c-set-style "bsd")
  (setq tab-width 4)
  (setq c-basic-offset tab-width))
(add-hook 'c++-mode-hook 'my-c++-mode-hook)
