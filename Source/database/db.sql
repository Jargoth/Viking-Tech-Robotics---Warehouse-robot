--
-- PostgreSQL database dump
--

-- Dumped from database version 10.20
-- Dumped by pg_dump version 10.20

-- Started on 2023-03-27 19:04:44

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 8 (class 2615 OID 965656)
-- Name: vikingtech; Type: SCHEMA; Schema: -; Owner: jargoth
--

CREATE SCHEMA vikingtech;


ALTER SCHEMA vikingtech OWNER TO jargoth;

--
-- TOC entry 246 (class 1259 OID 965663)
-- Name: robot_id_seq; Type: SEQUENCE; Schema: vikingtech; Owner: jargoth
--

CREATE SEQUENCE vikingtech.robot_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE vikingtech.robot_id_seq OWNER TO jargoth;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 245 (class 1259 OID 965657)
-- Name: robot; Type: TABLE; Schema: vikingtech; Owner: jargoth
--

CREATE TABLE vikingtech.robot (
    id integer DEFAULT nextval('vikingtech.robot_id_seq'::regclass) NOT NULL,
    text character varying(8) NOT NULL,
    complete boolean DEFAULT false NOT NULL
);


ALTER TABLE vikingtech.robot OWNER TO jargoth;

--
-- TOC entry 2935 (class 0 OID 965657)
-- Dependencies: 245
-- Data for Name: robot; Type: TABLE DATA; Schema: vikingtech; Owner: jargoth
--

COPY vikingtech.robot (id, text, complete) FROM stdin;
1	4C1	f
\.


--
-- TOC entry 2944 (class 0 OID 0)
-- Dependencies: 246
-- Name: robot_id_seq; Type: SEQUENCE SET; Schema: vikingtech; Owner: jargoth
--

SELECT pg_catalog.setval('vikingtech.robot_id_seq', 1, false);


--
-- TOC entry 2813 (class 2606 OID 965662)
-- Name: robot robot_pkey; Type: CONSTRAINT; Schema: vikingtech; Owner: jargoth
--

ALTER TABLE ONLY vikingtech.robot
    ADD CONSTRAINT robot_pkey PRIMARY KEY (id);


--
-- TOC entry 2942 (class 0 OID 0)
-- Dependencies: 8
-- Name: SCHEMA vikingtech; Type: ACL; Schema: -; Owner: jargoth
--

GRANT ALL ON SCHEMA vikingtech TO web;


--
-- TOC entry 2943 (class 0 OID 0)
-- Dependencies: 245
-- Name: TABLE robot; Type: ACL; Schema: vikingtech; Owner: jargoth
--

GRANT ALL ON TABLE vikingtech.robot TO web;


-- Completed on 2023-03-27 19:04:44

--
-- PostgreSQL database dump complete
--

