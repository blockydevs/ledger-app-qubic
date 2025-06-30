import pytest

from application_client.boilerplate_transaction_qubic import TransactionQubic
from application_client.boilerplate_command_sender import BoilerplateCommandSender, Errors
from application_client.boilerplate_response_unpacker import unpack_get_public_key_response, unpack_sign_tx_response
from ragger.error import ExceptionRAPDU
from ragger.navigator import NavInsID, NavIns

def enable_expert_mode(firmware, navigator):
    if firmware.name == 'flex':
        instructions = [
            NavInsID.USE_CASE_HOME_SETTINGS,
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavIns(NavInsID.TOUCH, (200, 300)),
            NavInsID.USE_CASE_SETTINGS_NEXT,
            NavIns(NavInsID.TOUCH, (200, 113))
        ]
    elif firmware.name == 'stax':
        instructions = [
            NavInsID.USE_CASE_HOME_SETTINGS,
            NavIns(NavInsID.TOUCH, (200, 113)),
            NavIns(NavInsID.TOUCH, (200, 261)),
            NavIns(NavInsID.TOUCH, (200, 400))
        ]
    else:
        instructions = [
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.BOTH_CLICK,
            NavInsID.RIGHT_CLICK,
            NavInsID.BOTH_CLICK
        ]

    navigator.navigate(instructions,
                       screen_change_before_first_instruction=False)


def test_sign_tx_expert_mode(firmware, backend, scenario_navigator, navigator):

    # given
    enable_expert_mode(firmware, navigator)

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/83293'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    destination_address = b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'

    source_public_key =bytes.fromhex("9F8C8F910AA2BB32AFFC3E6F0778490CD3722F5D90995990915790C71993852C")

    example_transaction = TransactionQubic(
        source_public_key=source_public_key,
        destination_public_key=destination_address,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with client.sign_tx(path=path, transaction=transaction_qubic):
        scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert public_key == b'\x8c\x8f\x91\n\xa2\xbb2\xaf\xfc>o\x07xI\x0c\xd3r/]\x90\x99Y\x90\x91W\x90\xc7\x19\x93\x85,'


def test_sign_tx_short_tx(backend, scenario_navigator):

    # given

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/83293'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    destination_address = b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'

    source_public_key =bytes.fromhex("9F8C8F910AA2BB32AFFC3E6F0778490CD3722F5D90995990915790C71993852C")

    example_transaction = TransactionQubic(
        source_public_key=source_public_key,
        destination_public_key=destination_address,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with client.sign_tx(path=path, transaction=transaction_qubic):
        scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert public_key == b'\x8c\x8f\x91\n\xa2\xbb2\xaf\xfc>o\x07xI\x0c\xd3r/]\x90\x99Y\x90\x91W\x90\xc7\x19\x93\x85,'

def test_sign_tx_short_tx_blind_sign(firmware, navigator, backend, scenario_navigator, test_name, default_screenshot_path):

    # given
    enable_expert_mode(firmware, navigator)

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/83293'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transaction = b'\x80\x03}q\x00X\x03\x00\x00\x00fooq\x01X\x03\x00\x00\x00barq\x02s.'

    # when

    if firmware.is_nano:
        with client.sign_tx(path=path, transaction=transaction):
            navigator.navigate_and_compare(default_screenshot_path,
                                            test_name+"/part1",
                                            [NavInsID.RIGHT_CLICK],
                                            screen_change_after_last_instruction=False)
            scenario_navigator.review_approve()

    else:
        with client.sign_tx(path=path, transaction=transaction):
            navigator.navigate_and_compare(default_screenshot_path,
                                           test_name+"/part1",
                                           [NavInsID.USE_CASE_CHOICE_REJECT],
                                           screen_change_after_last_instruction=False)
            scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert public_key == b'\x8c\x8f\x91\n\xa2\xbb2\xaf\xfc>o\x07xI\x0c\xd3r/]\x90\x99Y\x90\x91W\x90\xc7\x19\x93\x85,'

def test_sign_tx_long_tx(backend, scenario_navigator):

    # given

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/83293'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    print("rapdu: ")
    print(rapdu)
    print("rapdu.data: ")
    print(rapdu.data)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    destination_address = b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'

    source_public_key =bytes.fromhex("9F8C8F910AA2BB32AFFC3E6F0778490CD3722F5D90995990915790C71993852C")

    example_transaction = TransactionQubic(
        source_public_key=source_public_key,
        destination_public_key=destination_address,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with client.sign_tx(path=path, transaction=transaction_qubic):
        scenario_navigator.review_approve()

    # then

    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert public_key == b'\x8c\x8f\x91\n\xa2\xbb2\xaf\xfc>o\x07xI\x0c\xd3r/]\x90\x99Y\x90\x91W\x90\xc7\x19\x93\x85,'

def test_sign_tx_refused(backend, scenario_navigator):

    # given

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/83293'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, pub_key = unpack_get_public_key_response(rapdu.data)

    destination_address = b'\x04K\xfe\xc5||\xfbQR\xd6HD\x9e\x90\xf0\xfco\x8b;\xcf\x9bu\xc6\xef\xb1\xea\x81'

    source_public_key =bytes.fromhex("9F8C8F910AA2BB32AFFC3E6F0778490CD3722F5D90995990915790C71993852C")

    example_transaction = TransactionQubic(
        source_public_key=source_public_key,
        destination_public_key=destination_address,
        amount=123,
        tick=12345,
        input_type=1,
        input_size=256
    )
    transaction_qubic = example_transaction.serialize()

    # when

    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_tx(path=path, transaction=transaction_qubic):
            scenario_navigator.review_reject()

    # then

    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0

def test_sign_tx_short_tx_blind_sign_refused(firmware, navigator, backend, scenario_navigator,
                                             test_name, default_screenshot_path):
    # given
    enable_expert_mode(firmware, navigator)

    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/83293'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transaction = b'\x80\x03}q\x00X\x03\x00\x00\x00fooq\x01X\x03\x00\x00\x00barq\x02s.'

    # when

    if firmware.is_nano:
        with pytest.raises(ExceptionRAPDU) as e:
            with client.sign_tx(path=path, transaction=transaction):
                navigator.navigate_and_compare(default_screenshot_path,
                                               test_name+"/part1",
                                               [NavInsID.RIGHT_CLICK],
                                               screen_change_after_last_instruction=False)
                scenario_navigator.review_reject()

    else:
        with pytest.raises(ExceptionRAPDU) as e:
            with client.sign_tx(path=path, transaction=transaction):
                navigator.navigate_and_compare(default_screenshot_path,
                                               test_name+"/part1",
                                               [NavInsID.USE_CASE_CHOICE_REJECT],
                                               screen_change_after_last_instruction=False)
                scenario_navigator.review_reject()

    # then

    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0